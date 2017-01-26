// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <VoipViewController.h>

#import <Voip.h>
#import <objc/Ice.h>
#import <objc/Glacier2.h>

//
// Avoid warning for undocumented UISlider method
//
@interface UISlider(UndocumentedAPI)
-(void)setShowValue:(BOOL)val;
@end

@interface VoipViewController()

@property (nonatomic) UITextField* currentField;
@property (nonatomic) NSString* oldFieldValue;
@property (nonatomic) id<ICECommunicator> communicator;

@property (nonatomic) id<VoipSessionPrx> session;
@property (nonatomic) id<GLACIER2RouterPrx> router;

@property (nonatomic) NSTimer* refreshTimer;
@end

@implementation VoipViewController

@synthesize currentField;
@synthesize oldFieldValue;
@synthesize communicator;
@synthesize session;
@synthesize router;
@synthesize refreshTimer;

static NSString* usernameKey = @"usernameKey";
static NSString* passwordKey = @"passwordKey";

+(void)initialize
{
    NSDictionary* appDefaults = [NSDictionary dictionaryWithObjectsAndKeys: @"", usernameKey,
                                 @"", passwordKey,
                                 nil];

    [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
}

- (void)viewDidLoad {

    // Register IceSSL plugin and load it on communicator initialization.
    ICEregisterIceSSL(YES);

    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

    // Set the default values, and show the clear button in the text field.
    usernameField.text =  [defaults stringForKey:usernameKey];
    usernameField.clearButtonMode = UITextFieldViewModeWhileEditing;
    passwordField.text = [defaults stringForKey:passwordKey];
    passwordField.clearButtonMode = UITextFieldViewModeWhileEditing;

	callButton.enabled = NO;
    [callButton setAlpha:0.5];

	[[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didEnterBackground)
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];

	[[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(willEnterForeground)
                                                 name:UIApplicationWillEnterForegroundNotification
                                               object:nil];

    loginButton.enabled = usernameField.text.length > 0;
    [loginButton setAlpha:loginButton.enabled ? 1.0 : 0.5];
     // This generates a compile time warning, but does actually work!
    [delaySlider setShowValue:YES];

    delaySlider.minimumValue = 0;
    delaySlider.maximumValue = 10;
    delaySlider.value = 0.0;

    [super viewDidLoad];

    UIDevice* device = [UIDevice currentDevice];
    BOOL backgroundSupported = NO;
    if ([device respondsToSelector:@selector(isMultitaskingSupported)])
        backgroundSupported = device.multitaskingSupported;
    if(!backgroundSupported)
    {
        UIAlertView *alert = [[UIAlertView alloc]
                               initWithTitle:@"Warning"
                               message:@"The device doesn't support background applications, the application will terminate when moved to background."
                               delegate:nil
                               cancelButtonTitle:@"OK"
                               otherButtonTitles:nil];
        [alert show];
    }
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

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

	// Release any cached data, images, etc that aren't in use.
}


#pragma mark Session Management

-(void)destroySession
{
    UIApplication* app = [UIApplication sharedApplication];
    if(app.applicationState ==  UIApplicationStateBackground)
    {
        [app clearKeepAliveTimeout];
    }
    else
    {
        // Invalidate the refresh timer.
        [refreshTimer invalidate];
        self.refreshTimer = nil;
    }
    self.session = nil;

    callButton.enabled = NO;
    [callButton setAlpha:0.5];
    loginButton.enabled = usernameField.text.length > 0;
    [loginButton setAlpha:loginButton.enabled ? 1.0 : 0.5];
    [loginButton setTitle:@"Login" forState:UIControlStateNormal];

    // Destroy the session and destroy the communicator from another thread since these
    // calls block.
    id<ICECommunicator> c = communicator;
    id<GLACIER2RouterPrx> r = router;
    self.router = nil;
    self.communicator = nil;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        @try
        {
            [r destroySession];
        }
        @catch (ICEException* ex) {
        }

        @try
        {
            [c destroy];
        }
        @catch (ICEException* ex) {
        }
    });
}

-(void)exception:(NSString*)s
{
    [self connecting:NO];
    [self destroySession];

    UIApplication* app = [UIApplication sharedApplication];
    if(app.applicationState != UIApplicationStateBackground)
    {
        // open an alert with just an OK button
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                         message:s
                                                        delegate:nil
                                               cancelButtonTitle:@"OK"
                                               otherButtonTitles:nil];
        [alert show];
    }
}

-(void)refreshSession:(NSTimer*)timer
{
	if(session != nil)
	{
		[session begin_refresh:nil exception:^(ICEException* ex) { [self exception:[ex description]]; }];
	}
}

-(void)setupRefresh:(BOOL)background
{
    NSAssert(session != nil, @"sess is nil");

    // Timeout. Minimum is 601 seconds.
    ICELong timeout = sessionTimeout/2;
    if(timeout <= 600) {
        timeout = 601;
    }

    //
    // We use __unsafe_unretained here to avoid creating an ARC Retain cycle
    // in the block bellow.
    //
    UIApplication* __unsafe_unretained app = [UIApplication sharedApplication];
    if(background)
    {
        // Setup the session refresh timer.
		[app setKeepAliveTimeout:sessionTimeout/2 handler:^{
			// Note that this is blocking.
			if(session != nil)
			{
				@try
				{
					[session refresh];
				}
				@catch(ICEException* ex)
				{
                    [self destroySession];
                    if(app.applicationState ==  UIApplicationStateBackground)
                    {
                        UILocalNotification *localNotif = [[UILocalNotification alloc] init];
                        if (localNotif != nil)
                        {
                            localNotif.alertBody = [NSString stringWithFormat:@"Lost connection: %@.", [ex description]];

                            localNotif.soundName = UILocalNotificationDefaultSoundName;

                            [app presentLocalNotificationNow:localNotif];
                        }
                        else
                        {
                            // open an alert with just an OK button
                            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Call"
                                                                             message:[NSString stringWithFormat:@"Lost connection: %@.", [ex description]]
                                                                            delegate:nil
                                                                   cancelButtonTitle:@"OK"
                                                                   otherButtonTitles:nil];
                            [alert show];

                        }
                    }
                }
			}
		}];
    }
    else
    {
        // Setup the session refresh timer.
		self.refreshTimer = [NSTimer timerWithTimeInterval:sessionTimeout/2
													target:self
												  selector:@selector(refreshSession:)
												  userInfo:nil
												   repeats:YES];
		[[NSRunLoop currentRunLoop] addTimer:refreshTimer forMode:NSDefaultRunLoopMode];
    }
}

#pragma mark State transition

- (void)didEnterBackground
{
	// Disable the refresh timer.
	[refreshTimer invalidate];
    self.refreshTimer = nil;

    if(session != nil)
    {
        [self setupRefresh:YES];
    }
}

- (void)willEnterForeground
{
	// Disable the keep alive timer.
	[[UIApplication sharedApplication] clearKeepAliveTimeout];
    if(session != nil)
    {
        [self setupRefresh:NO];
    }
}

#pragma mark Text Field support

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

#pragma mark Login

-(IBAction)login:(id)sender
{
    // Logout.
	if(communicator != nil)
	{
        [self destroySession];
		return;
	}
    
    ICEInitializationData* initData = [ICEInitializationData initializationData];

    initData.properties = [ICEUtil createProperties];
    [initData.properties load:[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"config.client"]];
    [initData.properties setProperty:@"Ice.Voip" value:@"1"];
    [initData.properties setProperty:@"Ice.ACM.Client.Timeout" value:@"0"];
    [initData.properties setProperty:@"Ice.RetryIntervals" value:@"-1"];

    // Tracing properties.
    //[self.initData.properties setProperty:@"Ice.Trace.Network" value:@"1"];
    //[self.initData.properties setProperty:@"Ice.Trace.Protocol" value:@"1"];

    initData.dispatcher = ^(id<ICEDispatcherCall> call, id<ICEConnection> con) {
        dispatch_sync(dispatch_get_main_queue(), ^ { [call run]; });
    };

    NSAssert(communicator == nil, @"communicator == nil");

    @try
    {
        self.communicator = [ICEUtil createCommunicator:initData];
    }
    @catch(ICEEndpointParseException* ex)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error parsing config"
                                                         message:ex.reason
                                                        delegate:nil
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
            id<GLACIER2RouterPrx> r = [GLACIER2RouterPrx checkedCast:[communicator getDefaultRouter]];
            id<GLACIER2SessionPrx> glacier2session = [r createSession:usernameField.text password:passwordField.text];
            id<VoipSessionPrx> s = [VoipSessionPrx uncheckedCast:glacier2session];
            ICELong timeout = [r getSessionTimeout];

            id<ICEObjectAdapter> adapter = [communicator createObjectAdapterWithRouter:@"VoipClient"
                                                                                router:r];
            [adapter activate];

            VoipControl* callbackImpl = [VoipControl objectWithDelegate:self];

            ICEIdentity* callbackId = [ICEIdentity identity:[ICEUtil generateUUID] category:[r getCategoryForClient]];

            // Register the chat callback.
            [s setControl:[VoipControlPrx uncheckedCast:[adapter add:callbackImpl identity:callbackId]]];

            dispatch_async(dispatch_get_main_queue(), ^ {

                self.router = r;
                self.session = s;
                sessionTimeout = timeout;

                [self connecting:NO];

                [loginButton setTitle:@"Logout" forState:UIControlStateNormal];
                callButton.enabled = YES;
                [callButton setAlpha:1.0];

                [self setupRefresh:[UIApplication sharedApplication].applicationState == UIApplicationStateBackground];
            });
        }
        @catch(GLACIER2CannotCreateSessionException* ex)
        {
            NSString* s = [NSString stringWithFormat:@"Session creation failed: %@", ex.reason_];
            dispatch_async(dispatch_get_main_queue(), ^ { [self exception:s]; });
        }
        @catch(GLACIER2PermissionDeniedException* ex)
        {
            NSString* s = [NSString stringWithFormat:@"Login failed: %@", ex.reason_];
            dispatch_async(dispatch_get_main_queue(), ^ { [self exception:s]; });
        }
        @catch(ICEException* ex)
        {
            dispatch_async(dispatch_get_main_queue(), ^ { [self exception:[ex description]]; });
        }
    });
}


#pragma mark Call

-(IBAction)call:(id)sender
{
    if(session != nil)
    {
        int delay = (int)(delaySlider.value * 1000.0f); // Convert to ms.

        [session begin_simulateCall:delay response:nil exception:^(ICEException* ex) { [self exception:[ex description]]; }];
    }
}

#pragma mark VoipControl

-(void)incomingCall:(ICECurrent*)current
{
    NSDate* now = [NSDate date];

    UIApplication* app = [UIApplication sharedApplication];

    //
    // Create a date formatter configured for local time zone.
    //
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    formatter.dateFormat = @"yyyy-MM-dd HH:mm";
    [formatter setTimeZone:[NSTimeZone systemTimeZone]];

    if(app.applicationState ==  UIApplicationStateBackground)
    {
        UILocalNotification *localNotif = [[UILocalNotification alloc] init];
        if (localNotif != nil)
        {
            localNotif.alertBody = [NSString stringWithFormat:@"Incoming call at %@.", [formatter stringFromDate:now]];

            localNotif.soundName = UILocalNotificationDefaultSoundName;

            [[UIApplication sharedApplication] presentLocalNotificationNow:localNotif];
        }
    }
    else
    {
        // open an alert with just an OK button
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Call"
                                                        message:[NSString stringWithFormat:@"Incoming call at %@.",
                                                                                            [formatter stringFromDate:now]]
                                                        delegate:nil
                                               cancelButtonTitle:@"OK"
                                               otherButtonTitles:nil];
        [alert show];
    }
}
@end
