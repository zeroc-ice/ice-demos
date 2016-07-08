// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <LoginController.h>
#import <ChatController.h>

#import <objc/Ice.h>
#import <objc/Glacier2.h>

@interface LoginController()

@property (nonatomic) UITextField* currentField;
@property (nonatomic) NSString* oldFieldValue;
@property (nonatomic) WaitAlert* waitAlert;
@property (nonatomic) id<ICECommunicator> communicator;

@end

@implementation LoginController

@synthesize currentField;
@synthesize oldFieldValue;
@synthesize waitAlert;
@synthesize communicator;

static NSString* usernameKey = @"usernameKey";
static NSString* passwordKey = @"passwordKey";
static NSString* sslKey = @"sslKey";

+(void)initialize
{
    NSDictionary* appDefaults = [NSDictionary dictionaryWithObjectsAndKeys:@"", usernameKey,
                                 @"", passwordKey,
                                 @"YES", sslKey,
                                 nil];
    
    [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
}

- (void)viewDidLoad
{
    // Register IceSSL plugin and load it on communicator initialization.
    ICEregisterIceSSL(YES);

    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

    // Set the default values, and show the clear button in the text field.
    usernameField.text =  [defaults stringForKey:usernameKey];
    usernameField.clearButtonMode = UITextFieldViewModeWhileEditing;
    passwordField.text = [defaults stringForKey:passwordKey];
    passwordField.clearButtonMode = UITextFieldViewModeWhileEditing;
    
    chatController = [[ChatController alloc] initWithNibName:@"ChatView" bundle:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationDidEnterBackground) 
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];
}

-(void) connecting:(BOOL)v
{
    // Show the wait alert.
    statusLabel.text = @"Connecting...";

    statusLabel.hidden = !v;
    if(v)
    {
        [statusActivity startAnimating];
    }
    else
    {
        [statusActivity stopAnimating];
    }
    loginButton.enabled = !v;
    usernameField.enabled = !v;
    passwordField.enabled = !v;
}

- (void)applicationDidEnterBackground
{
    // Start the long-running task and return immediately.
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [communicator destroy];
    });
}

- (void)viewWillAppear:(BOOL)animated
{
    loginButton.enabled = usernameField.text.length > 0;
    [loginButton setAlpha:loginButton.enabled ? 1.0 : 0.5];
	[super viewWillAppear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}

#pragma mark UITextFieldDelegate

-(BOOL)textFieldShouldBeginEditing:(UITextField*)field
{
    self.currentField = field;
    self.oldFieldValue = field.text;
    return YES;
}

-(BOOL)textFieldShouldReturn:(UITextField*)theTextField
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

    return YES;
}

#pragma mark -

// A touch outside the keyboard dismisses the keyboard, and
// sets back the old field value.
-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    [currentField resignFirstResponder];
    currentField.text = oldFieldValue; 
    self.currentField = nil;
    [super touchesBegan:touches withEvent:event];
}

#pragma mark UI Actions

-(IBAction)sslChanged:(id)s
{
    UISwitch* sender = (UISwitch*)s;
    [[NSUserDefaults standardUserDefaults] setBool:sender.isOn forKey:sslKey];
}

#pragma mark Login

-(void)exception:(NSString*)s
{
    [self connecting:FALSE];

    // We always create a new communicator each time
    // we try to login.
    [communicator destroy];
    self.communicator = nil;
 
    loginButton.enabled = usernameField.text.length > 0;
    [loginButton setAlpha:loginButton.enabled ? 1.0 : 0.5];
    
    // open an alert with just an OK button
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                     message:s
                                                    delegate:nil
                                           cancelButtonTitle:@"OK"
                                           otherButtonTitles:nil];
    [alert show];       
}

-(IBAction)login:(id)sender
{
    ICEInitializationData* initData = [ICEInitializationData initializationData];
    
    initData.properties = [ICEUtil createProperties];
    [initData.properties load:[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"config.client"]];
    [initData.properties setProperty:@"Ice.ACM.Client.Timeout" value:@"0"];
    [initData.properties setProperty:@"Ice.RetryIntervals" value:@"-1"];
    
    initData.dispatcher = ^(id<ICEDispatcherCall> call, id<ICEConnection> con)
    {
        dispatch_sync(dispatch_get_main_queue(), ^ { [call run]; });
    };
    
    NSAssert(communicator == nil, @"communicator == nil");
    self.communicator = [ICEUtil createCommunicator:initData];
    
    [self connecting:TRUE];
    
    NSString* username = usernameField.text;
    NSString* password = passwordField.text;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        @try
        {
            id<GLACIER2RouterPrx> router = [GLACIER2RouterPrx checkedCast:[communicator getDefaultRouter]];
            id<GLACIER2SessionPrx> glacier2session = [router createSession:username password:password];
            id<ChatChatSessionPrx> sess = [ChatChatSessionPrx uncheckedCast:glacier2session];
            
            ICEInt acmTiemout = [router getACMTimeout];
            if(acmTiemout <= 0)
            {
                acmTiemout = (ICEInt)[router getSessionTimeout];
            }
            
            [chatController setup:communicator
                          session:sess
                       acmTimeout:acmTiemout
                           router:router
                         category:[router getCategoryForClient]];
            dispatch_async(dispatch_get_main_queue(), ^ {
                [self connecting:FALSE];
                
                // The communicator is now owned by the ChatController.
                self.communicator = nil;
                
                [chatController activate:@"Chat"];
                [self.navigationController pushViewController:chatController animated:YES];
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
        @catch(ICEEndpointParseException* ex)
        {
            NSString* s = [NSString stringWithFormat:@"Invalid router: %@", ex.reason];
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
    });
}

@end

