// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <LoginController.h>
#import <MainController.h>

#import <Library.h>
#import <Session.h>
#import <Glacier2Session.h>

#import <objc/Glacier2.h>

@interface LoginController()

@property (nonatomic) UITextField* currentField;
@property (nonatomic) NSString* oldFieldValue;
@property (nonatomic) MainController *mainController;

@property (nonatomic) id<ICECommunicator> communicator;
@property (nonatomic) id session;
@property (nonatomic) id<DemoLibraryPrx> library;
@property (nonatomic) id<GLACIER2RouterPrx> router;
@property (nonatomic) ICEInitializationData* initializationData;

@end

@implementation LoginController

@synthesize currentField;
@synthesize oldFieldValue;
@synthesize mainController;
@synthesize session;
@synthesize library;
@synthesize router;
@synthesize communicator;
@synthesize initializationData;

static NSString* usernameKey = @"usernameKey";
static NSString* passwordKey = @"passwordKey";

+(void)initialize
{
    NSDictionary* appDefaults = [NSDictionary dictionaryWithObjectsAndKeys:@"", usernameKey,
                                 @"", passwordKey,
                                 nil];

    [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
}

-(void)viewDidLoad
{
    // Register IceSSL/IceWS plugins and load them on communicator initialization.
    ICEregisterIceSSL(YES);
    ICEregisterIceWS(YES);

    //Load initialization data here so that we check if the username/passwords fields should be hidden.
    initializationData = [ICEInitializationData initializationData];
    initializationData.properties = [ICEUtil createProperties];
    [initializationData.properties load:[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"config.client"]];
    [initializationData.properties setProperty:@"Ice.RetryIntervals" value:@"-1"];

    initializationData.dispatcher = ^(id<ICEDispatcherCall> call, id<ICEConnection> con)
    {
        dispatch_sync(dispatch_get_main_queue(), ^ { [call run]; });
    };

    // If Ice.Default.Router is not set then we hide the login and password prompts.
    if([[[initializationData properties] getProperty:@"Ice.Default.Router"] length] == 0)
    {
        [[usernameField superview] setHidden:YES];
        [[passwordField superview] setHidden:YES];
        [loginButton setEnabled:YES];
    }
    else
    {
        NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

        // Set the default values
        usernameField.text = [defaults stringForKey:usernameKey];
        passwordField.text = [defaults stringForKey:passwordKey];
    }

    UIStoryboard*  mainStoryboard = [UIStoryboard storyboardWithName:@"Main"
                                                             bundle: nil];
    mainController = (MainController*)[mainStoryboard instantiateViewControllerWithIdentifier:@"MainController"];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationWillTerminate)
                                                 name:UIApplicationWillTerminateNotification
                                               object:nil];
}

-(void)applicationWillTerminate
{
    [communicator destroy];
}

- (void)viewWillAppear:(BOOL)animated
{
    loginButton.enabled = (usernameField.text.length > 0) || ([[usernameField superview] isHidden] == YES);
    [loginButton setAlpha:loginButton.enabled ? 1.0 : 0.5];
    [super viewWillAppear:animated];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

#pragma mark UITextFieldDelegate

- (void)textFieldDidBeginEditing:(UITextField *)field;
{
    self.currentField = field;
    self.oldFieldValue = field.text;
}

- (void)textFieldDidEndEditing:(UITextField *)theTextField;
{
    NSAssert(theTextField == currentField, @"theTextField == currentTextField");

    // When the user presses return, take focus away from the text
    // field so that the keyboard is dismissed.
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    if(theTextField == usernameField)
    {
        [defaults setObject:theTextField.text forKey:usernameKey];
    }
    else if(theTextField == passwordField)
    {
        [defaults setObject:theTextField.text forKey:passwordKey];
    }

    loginButton.enabled = usernameField.text.length > 0;
    [loginButton setAlpha:loginButton.enabled ? 1.0 : 0.5];

    [theTextField resignFirstResponder];
    self.currentField = nil;
}

#pragma mark -

// A touch outside the keyboard dismisses the keyboard, and
// sets back the old field value.
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [currentField resignFirstResponder];
    currentField.text = oldFieldValue;
    self.currentField = nil;
    [super touchesBegan:touches withEvent:event];
}

#pragma mark Login

-(void)exception:(NSString*)s
{
    [self connecting:NO];

    // We always create a new communicator each time
    // we try to login.
    [communicator destroy];
    self.communicator = nil;

    // open an alert with just an OK button
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Error"
                                                                   message:s
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];
    [self presentViewController:alert animated:YES completion:nil];
}

-(void)connecting:(BOOL)connecting
{
    statusLabel.hidden = !connecting;
    if(connecting)
    {
        [statusActivity startAnimating];
    }
    else
    {
        [statusActivity stopAnimating];
    }
    usernameField.enabled = !connecting;
    passwordField.enabled = !connecting;
    loginButton.enabled = !connecting;
}

// Runs in a separate thread, called only by NSInvocationOperation.
-(void)doLogin:(id)proxy
{
    id<DemoSessionFactoryPrx> factory = [DemoSessionFactoryPrx checkedCast:proxy];
    if(factory == nil)
    {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"Invalid proxy" userInfo:nil];
    }

    id<DemoSessionPrx> sess = [factory create];

    self.session = sess;
    self.library = [sess getLibrary];
}

// Runs in a separate thread, called only by NSInvocationOperation.
-(void)doGlacier2Login:(id)proxy username:(NSString*)username password:(NSString*)password
{
    id<GLACIER2RouterPrx> glacier2router = [GLACIER2RouterPrx uncheckedCast:proxy];

    id<GLACIER2SessionPrx> glacier2session = [glacier2router createSession:username password:password];
    id<DemoGlacier2SessionPrx> sess = [DemoGlacier2SessionPrx uncheckedCast:glacier2session];

    self.session = sess;
    self.router = glacier2router;
    self.library = [sess getLibrary];

    ICELong acmTimeout = [glacier2router getACMTimeout];
    if(acmTimeout > 0)
    {
        //
        // Configure the connection to send heartbeats in order to keep our session alive
        //
        [[glacier2router ice_getCachedConnection] setACM:@(acmTimeout) close:ICENone heartbeat:@(ICEHeartbeatAlways)];
    }
}

-(IBAction)login:(id)sender
{
    id proxy;
    @try
    {
        NSAssert(communicator == nil, @"communicator == nil");
        self.communicator = [ICEUtil createCommunicator:initializationData];

        if([[[communicator getProperties] getProperty:@"Ice.Default.Router"] length] > 0)
        {
            proxy = [communicator getDefaultRouter];
        }
        else
        {
            proxy = [communicator stringToProxy:[[communicator getProperties] getProperty:@"SessionFactory.Proxy"]];
        }
    }
    @catch(ICEEndpointParseException* ex)
    {
        UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Error parsing config"
                                                                       message:ex.reason
                                                                preferredStyle:UIAlertControllerStyleAlert];
        [alert addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];
        [self presentViewController:alert animated:YES completion:nil];

        [communicator destroy];
        self.communicator = nil;
        return;
    }
    @catch(...)
    {
        UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Error parsing config"
                                                                       message:@"Please check your config file"
                                                                preferredStyle:UIAlertControllerStyleAlert];
        [alert addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];
        [self presentViewController:alert animated:YES completion:nil];

        [communicator destroy];
        self.communicator = nil;
        return;
    }

    [self connecting:YES];

    NSString* username = usernameField.text;
    NSString* password = passwordField.text;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        @try
        {
            if([[[self.communicator getProperties] getProperty:@"Ice.Default.Router"] length] > 0)
            {
                [self doGlacier2Login:proxy username:username password:password];
            }
            else
            {
                [self doLogin:proxy];
            }

            dispatch_async(dispatch_get_main_queue(), ^ {
                [self connecting:NO];
                [self.mainController activate:self.communicator
                                      session:self.session
                                       router:self.router
                                      library:self.library];

                // Clear internal state.
                self.communicator = nil;
                self.session = nil;
                self.library = nil;
                self.router = nil;

                [self.navigationController pushViewController:self.mainController animated:YES];
            });
        }
        @catch(GLACIER2CannotCreateSessionException* ex)
        {
            NSString* s = [NSString stringWithFormat:@"Session creation failed: %@", ex.reason_];
            dispatch_async(dispatch_get_main_queue(), ^ {
                [self exception:s];
            });
        }
        @catch(GLACIER2PermissionDeniedException* ex)
        {
            NSString* s = [NSString stringWithFormat:@"Login failed: %@", ex.reason_];
            dispatch_async(dispatch_get_main_queue(), ^ {
                [self exception:s];
            });
        }
        @catch(ICEException* ex)
        {
            dispatch_async(dispatch_get_main_queue(), ^ {
                [self exception:[ex description]];
            });
        }
        @catch(NSException *ex)
        {
            dispatch_async(dispatch_get_main_queue(), ^ {
                [self exception:[ex reason]];
            });
        }
    });
}
@end
