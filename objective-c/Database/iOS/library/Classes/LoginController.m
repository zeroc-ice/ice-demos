// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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

@property (nonatomic) id<ICECommunicator> communicator;
@property (nonatomic) id session;
@property (nonatomic) id<DemoLibraryPrx> library;
@property (nonatomic) id<GLACIER2RouterPrx> router;

@end

@implementation LoginController

@synthesize currentField;
@synthesize oldFieldValue;
@synthesize session;
@synthesize library;
@synthesize router;
@synthesize communicator;

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
    // Register IceSSL plugin and load it on communicator initialization.
    ICEregisterIceSSL(YES);

    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    
    // Set the default values, and show the clear button in the text field.
    usernameField.clearButtonMode = UITextFieldViewModeWhileEditing;
    usernameField.text = [defaults stringForKey:usernameKey];
    passwordField.clearButtonMode = UITextFieldViewModeWhileEditing;
    passwordField.text = [defaults stringForKey:passwordKey];
    
    mainController = [[MainController alloc] initWithNibName:@"MainView" bundle:nil];
    
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
    loginButton.enabled = usernameField.text.length > 0;
    [loginButton setAlpha:loginButton.enabled ? 1.0 : 0.5];
	[super viewWillAppear:animated];
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

-(void)didReceiveMemoryWarning
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
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                     message:s
                                                    delegate:self
                                           cancelButtonTitle:@"OK"
                                           otherButtonTitles:nil];
    [alert show];
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
	sessionTimeout = [factory getSessionTimeout];
	self.library = [sess getLibrary];
}

// Runs in a separate thread, called only by NSInvocationOperation.
-(void)doGlacier2Login:(id)proxy
{
    id<GLACIER2RouterPrx> glacier2router = [GLACIER2RouterPrx uncheckedCast:proxy];
	
	id<GLACIER2SessionPrx> glacier2session = [glacier2router createSession:usernameField.text password:passwordField.text];
	id<DemoGlacier2SessionPrx> sess = [DemoGlacier2SessionPrx uncheckedCast:glacier2session];
	
	self.session = sess;
	self.router = glacier2router;
	sessionTimeout = [glacier2router getSessionTimeout];
	self.library = [sess getLibrary];
}

-(IBAction)login:(id)sender
{
    ICEInitializationData* initData = [ICEInitializationData initializationData];
    initData.properties = [ICEUtil createProperties];
    [initData.properties load:[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"config.client"]];
    [initData.properties setProperty:@"Ice.ACM.Client.Timeout" value:@"0"];
    [initData.properties setProperty:@"Ice.RetryIntervals" value:@"-1"];
    
    // Tracing properties.
    //[initData.properties setProperty:@"Ice.Trace.Network" value:@"1"];
    //[initData.properties setProperty:@"Ice.Trace.Protocol" value:@"1"];

    initData.dispatcher = ^(id<ICEDispatcherCall> call, id<ICEConnection> con) {
        dispatch_sync(dispatch_get_main_queue(), ^ { [call run]; });
    };
    
	
    id<ICEObjectPrx> proxy;
    SEL loginSelector;
    @try
    {
        NSAssert(communicator == nil, @"communicator == nil");
        self.communicator = [ICEUtil createCommunicator:initData];
        
        if([[[communicator getProperties] getProperty:@"Ice.Default.Router"] length] > 0)
        {
            proxy = [communicator getDefaultRouter];
            loginSelector = @selector(doGlacier2Login:);
        }
        else
        {
            proxy = [communicator stringToProxy:[[communicator getProperties] getProperty:@"SessionFactory.Proxy"]];
            loginSelector = @selector(doLogin:);
        }
    }
    @catch(ICEEndpointParseException* ex)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error parsing config"
                                                        message:ex.reason
                                                       delegate:self
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
        
        [communicator destroy];
        self.communicator = nil;
        return;
    }
    @catch(...)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error parsing config"
                                                        message:@"Please check your config file"
                                                       delegate:self
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
        
        [communicator destroy];
        self.communicator = nil;
        return;
    }
    
    [self connecting:YES];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        @try
        {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
			[self performSelector:loginSelector withObject:proxy];
#pragma clang diagnostic pop
			dispatch_async(dispatch_get_main_queue(), ^ {
                [self connecting:NO];
				[mainController activate:communicator
								 session:session
								  router:router
						  sessionTimeout:sessionTimeout
								 library:library];
				
				// Clear internal state.
				self.communicator = nil;
				self.session = nil;
				self.library = nil;
				self.router = nil;
				
				[self.navigationController pushViewController:mainController animated:YES];
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
