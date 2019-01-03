// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <LoginController.h>
#import <LibraryController.h>

#import <Library.h>
#import <Glacier2Session.h>
#import <Session.h>

#import <objc/Ice.h>
#import <objc/Glacier2.h>

NSString* const usernameKey = @"usernameKey";
NSString* const passwordKey = @"passwordKey";

@interface LoginController()
@property (nonatomic) NSWindow* connectingSheet;
@property (nonatomic)  NSProgressIndicator* progress;
@property (nonatomic) id<ICECommunicator> communicator;
@property (nonatomic) ICEInitializationData* initializationData;
@end

@implementation LoginController

@synthesize connectingSheet;
@synthesize progress;
@synthesize communicator;
@synthesize initializationData;

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
    // Register and load the IceSSL and IceWS plugins on communicator initialization.
    ICEregisterIceSSL(YES);
    ICEregisterIceWS(YES);

    initializationData = [ICEInitializationData initializationData];
    initializationData.properties = [ICEUtil createProperties];
    [initializationData.properties load:[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"config.client"]];
    [initializationData.properties setProperty:@"Ice.RetryIntervals" value:@"-1"];

    initializationData.dispatcher = ^(id<ICEDispatcherCall> call, id<ICEConnection> con)
    {
        dispatch_sync(dispatch_get_main_queue(), ^ { [call run]; });
    };

    [initializationData.properties setProperty:@"IceSSL.DefaultDir" value:[[NSBundle mainBundle] resourcePath]];

    if([[[initializationData properties] getProperty:@"Ice.Default.Router"] length] == 0)
    {
        [usernameField setHidden:YES];
        [passwordField setHidden:YES];
    }
    else
    {
        // Initialize the fields from the application defaults.
        NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];

        usernameField.stringValue = [defaults stringForKey:usernameKey];
        passwordField.stringValue = [defaults stringForKey:passwordKey];
    }

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
    id<DemoLibraryPrx> library = [session getLibrary];
    return [[LibraryController alloc]
            initWithCommunicator:[proxy ice_getCommunicator]
            session:session
            router:nil
            library:library];
}

// Direct login through Glacier2.
-(LibraryController*)doGlacier2Login:(id)proxy username:(NSString*)username password:(NSString*)password NS_RETURNS_RETAINED
{
    id<GLACIER2RouterPrx> router = [GLACIER2RouterPrx checkedCast:[communicator getDefaultRouter]];
    id<GLACIER2SessionPrx> glacier2session = [router createSession:username password:password];
    id<DemoGlacier2SessionPrx> session = [DemoGlacier2SessionPrx uncheckedCast:glacier2session];

    id<DemoLibraryPrx> library = [session getLibrary];

    ICELong acmTimeout = [router getACMTimeout];
    if(acmTimeout > 0)
    {
        //
        // Configure the connection to send heartbeats in order to keep our session alive
        //
        [[router ice_getCachedConnection] setACM:@(acmTimeout) close:ICENone heartbeat:@(ICEHeartbeatAlways)];
    }

    return [[LibraryController alloc]
            initWithCommunicator:[proxy ice_getCommunicator]
            session:session
            router:router
            library:library];
}

#pragma mark Login

-(void)login:(id)sender
{
    id proxy;
    @try
    {
        NSAssert(communicator == nil, @"communicator == nil");
        communicator = [ICEUtil createCommunicator:initializationData];
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

    NSString* username = usernameField.stringValue;
    NSString* password = passwordField.stringValue;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        NSString* msg;
        @try
        {
            LibraryController* libraryController;
            if([[[self.communicator getProperties] getProperty:@"Ice.Default.Router"] length] > 0)
            {
                libraryController = [self doGlacier2Login:proxy username:username password:password];
            }
            else
            {
                libraryController = [self doLogin:proxy];
            }

            dispatch_async(dispatch_get_main_queue(), ^ {
                // Hide the connecting sheet.
                [NSApp endSheet:self.connectingSheet];
                [self.connectingSheet orderOut:self.window];
                [self.progress stopAnimation:self];

                // The communicator is now owned by the LibraryController.
                self.communicator = nil;

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
            [NSApp endSheet:self.connectingSheet];
            [self.connectingSheet orderOut:self.window];
            [self.progress stopAnimation:self];

            [self.communicator destroy];
            self.communicator = nil;

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
    [NSApp endSheet:advancedSheet];
    [advancedSheet orderOut:sender];
}

@end
