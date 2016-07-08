// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <LoginController.h>
#import <LibraryController.h>

#import <Library.h>
#import <Glacier2Session.h>
#import <Session.h>
#import <Router.h>

#import <objc/Ice.h>

NSString* const usernameKey = @"usernameKey";
NSString* const passwordKey = @"passwordKey";

@implementation LoginController

+(void)initialize
{
    // Initialize the application defaults.
    NSDictionary* appDefaults = [NSDictionary dictionaryWithObjectsAndKeys:
                                 @"", usernameKey,
                                 @"", passwordKey,
                                 nil];

    [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];

}

- (id)init
{
    return [super initWithWindowNibName:@"LoginView"];
}

-(void)awakeFromNib
{
    // Register and load the IceSSL plugin on communicator initialization.
    ICEregisterIceSSL(YES);
    
    // Initialize the fields from the application defaults.
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

    usernameField.stringValue = [defaults stringForKey:usernameKey];
    passwordField.stringValue = [defaults stringForKey:passwordKey];

}

#pragma mark Login callbacks

// Direct login to the library server.
-(LibraryController*)doLogin:(id)proxy
{
    id<DemoSessionFactoryPrx> factory = [DemoSessionFactoryPrx checkedCast:proxy];
    if(factory == nil)
    {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"Invalid proxy" userInfo:nil];
    }

    id<DemoSessionPrx> session = [factory create];
    int sessionTimeout = [factory getSessionTimeout];
    id<DemoLibraryPrx> library = [session getLibrary];
    return [[LibraryController alloc]
            initWithCommunicator:[proxy ice_getCommunicator]
            session:session
            router:nil
            sessionTimeout:sessionTimeout
            library:library];
}

// Direct login through Glacier2.
-(LibraryController*)doGlacier2Login:(id)proxy NS_RETURNS_RETAINED
{
    id<GLACIER2RouterPrx> router = [GLACIER2RouterPrx checkedCast:[communicator getDefaultRouter]];
    id<GLACIER2SessionPrx> glacier2session = [router createSession:usernameField.stringValue
                                                          password:passwordField.stringValue];
    id<DemoGlacier2SessionPrx> session = [DemoGlacier2SessionPrx uncheckedCast:glacier2session];

    int sessionTimeout = [router getSessionTimeout];

    id<DemoLibraryPrx> library = [session getLibrary];

    return [[LibraryController alloc]
            initWithCommunicator:[proxy ice_getCommunicator]
            session:session
            router:router
            sessionTimeout:sessionTimeout
            library:library];
}

// Login through the iPhone router.
-(LibraryController*)doPhoneRouterLogin:(id)proxy NS_RETURNS_RETAINED
{
    id<DemoRouterPrx> router = [DemoRouterPrx uncheckedCast:[communicator getDefaultRouter]];
    [router createSession];

    id<DemoSessionFactoryPrx> factory = [DemoSessionFactoryPrx checkedCast:proxy];
    if(factory == nil)
    {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"Invalid proxy" userInfo:nil];
    }

    id<DemoSessionPrx> session = [factory create];
    int sessionTimeout = [factory getSessionTimeout];
    id<DemoLibraryPrx> library = [session getLibrary];
    return [[LibraryController alloc]
            initWithCommunicator:[proxy ice_getCommunicator]
            session:session
            router:nil
            sessionTimeout:sessionTimeout
            library:library];
}

// Login through the iPhone router, using Glacier2.
-(LibraryController*)doPhoneRouterGlacier2Login:(id)proxy NS_RETURNS_RETAINED
{
    id<DemoRouterPrx> router = [DemoRouterPrx uncheckedCast:[communicator getDefaultRouter]];
    id<ICERouterPrx> glacier2router = [ICERouterPrx uncheckedCast:proxy];
    id<GLACIER2SessionPrx> glacier2session;
    NSMutableString* category;
    ICELong sessionTimeout;

    [router createGlacier2Session:glacier2router
                           userId:usernameField.stringValue
                         password:passwordField.stringValue
                         category:&category
                   sessionTimeout:&sessionTimeout
                             sess:&glacier2session];
    id<DemoGlacier2SessionPrx> session = [DemoGlacier2SessionPrx uncheckedCast:glacier2session];
    id<DemoLibraryPrx> library = [session getLibrary];

    return [[LibraryController alloc] initWithCommunicator:[proxy ice_getCommunicator]
                                                   session:[DemoGlacier2SessionPrx uncheckedCast:glacier2session]
                                                    router:nil
                                            sessionTimeout:sessionTimeout
                                                   library:library];
}

#pragma mark Login

-(void)login:(id)sender
{
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:usernameField.stringValue forKey:usernameKey];
    [defaults setObject:passwordField.stringValue forKey:passwordKey];

    ICEInitializationData* initData = [ICEInitializationData initializationData];
    initData.properties = [ICEUtil createProperties];
    [initData.properties load:[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"config.client"]];
    [initData.properties setProperty:@"Ice.ACM.Client.Timeout" value:@"0"];
    [initData.properties setProperty:@"Ice.RetryIntervals" value:@"-1"];

    // Tracing properties.
    //[initData.properties setProperty:@"Ice.Trace.Network" value:@"1"];
    //[initData.properties setProperty:@"Ice.Trace.Protocol" value:@"1"];
    //[initData.properties setProperty:@"IceSSL.Trace.Security" value:@"1"];

    initData.dispatcher = ^(id<ICEDispatcherCall> call, id<ICEConnection> con)
    {
        dispatch_sync(dispatch_get_main_queue(), ^ { [call run]; });
    };

    [initData.properties setProperty:@"IceSSL.DefaultDir" value:[[NSBundle mainBundle] resourcePath]];

    SEL loginSelector;
    id<ICEObjectPrx> proxy;
    @try
    {
        NSAssert(communicator == nil, @"communicator == nil");
        communicator = [ICEUtil createCommunicator:initData];

        NSString* router = [[communicator getProperties] getProperty:@"Ice.Default.Router"];
        NSString* glacier2Proxy = [[communicator getProperties] getProperty:@"SessionFactory.Glacier2Proxy"];
        NSString* sessionFactoryProxy = [[communicator getProperties] getProperty:@"SessionFactory.Proxy"];

        int connectionType = [[communicator getProperties] getPropertyAsIntWithDefault:@"LibraryDemo.ConnectionType" value:-1];
        switch (connectionType) {
            case 1: // Login though Glacier2
                proxy = [communicator stringToProxy:router];
                loginSelector = @selector(doGlacier2Login:);
                break;
            case 2: // Login directly to Library server
                proxy = [communicator stringToProxy:sessionFactoryProxy];
                loginSelector = @selector(doLogin:);
                break;
            case 3: // Login though iPhone router, using Glacier2
                proxy = [communicator stringToProxy:glacier2Proxy];
                loginSelector = @selector(doPhoneRouterGlacier2Login:);
                break;
            case 4: // Login though iPhone router, directly to Library server
                proxy = [communicator stringToProxy:sessionFactoryProxy];
                loginSelector = @selector(doPhoneRouterLogin:);
                break;
            default:
                NSRunAlertPanel(@"Error", @"%@", @"OK", nil, nil, @"Please set configuration type in 'config.client' file.");
                break;
        }
    }
    @catch(ICEEndpointParseException* ex)
    {
        [communicator destroy];
        communicator = nil;

        NSRunAlertPanel(@"Error", @"%@", @"OK", nil, nil, [ex description]);
        return;
    }

    [NSApp beginSheet:connectingSheet
       modalForWindow:self.window
        modalDelegate:nil
       didEndSelector:NULL
          contextInfo:NULL];
    [progress startAnimation:self];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        NSString* msg;
        @try
        {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
            LibraryController* libraryController = [self performSelector:loginSelector withObject:proxy];
#pragma clang diagnostic pop
            dispatch_async(dispatch_get_main_queue(), ^ {
                // Hide the connecting sheet.
                [NSApp endSheet:connectingSheet];
                [connectingSheet orderOut:self.window];
                [progress stopAnimation:self];

                // The communicator is now owned by the LibraryController.
                communicator = nil;

                // Close the connecting window, show the main window.
                [self.window close];
                [libraryController showWindow:self];
            });
            return;
        }
        @catch(GLACIER2CannotCreateSessionException* ex)
        {
            msg = [NSString stringWithFormat:@"Session creation failed: %@", ex.reason_];
        }
        @catch(GLACIER2PermissionDeniedException* ex)
        {
            msg = [NSString stringWithFormat:@"Login failed: %@", ex.reason_];
        }
        @catch(ICEException* ex)
        {
            msg = [ex description];
        }
        @catch(NSException *ex)
        {
            msg = [ex reason];
        }

        dispatch_async(dispatch_get_main_queue(), ^ {
            // Hide the connecting sheet.
            [NSApp endSheet:connectingSheet];
            [connectingSheet orderOut:self.window];
            [progress stopAnimation:self];

            [communicator destroy];
            communicator = nil;

            NSRunAlertPanel(@"Error", @"%@", @"OK", nil, nil, msg);
        });
    });
}

-(void)showAdvancedSheet:(id)sender
{
    [NSApp beginSheet:advancedSheet
       modalForWindow:self.window
        modalDelegate:nil
       didEndSelector:NULL
          contextInfo:NULL];
}

-(void)closeAdvancedSheet:(id)sender
{
//    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

    [NSApp endSheet:advancedSheet];
    [advancedSheet orderOut:sender];
}

@end
