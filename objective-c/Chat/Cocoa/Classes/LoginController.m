// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <LoginController.h>
#import <ChatController.h>

#import <Chat.h>
#import <Router.h>

#import <objc/Ice.h>
#import <objc/Glacier2.h>

NSString* const usernameKey = @"usernameKey";
NSString* const passwordKey = @"passwordKey";

@interface LoginController()
@property (nonatomic) NSWindow* connectingSheet;
@property (nonatomic)  NSProgressIndicator* progress;
@property (nonatomic) id<ICECommunicator> communicator;
@end

@implementation LoginController

@synthesize connectingSheet;
@synthesize progress;
@synthesize communicator;

// Initialize the app defaults.
+(void)initialize
{
    // Initialize the application defaults.
    NSDictionary* appDefaults = [NSDictionary dictionaryWithObjectsAndKeys:
                                 @"", usernameKey,
                                 @"", passwordKey,
                                 nil];

    [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
}

-(id)init
{
    return [super initWithWindowNibName:@"LoginView"];
}

-(void)awakeFromNib
{
    // Register IceSSL/IceWS plugins and load them on communicator initialization.
    ICEregisterIceSSL(YES);
    ICEregisterIceWS(YES);

    // Restore the field values from the app defaults.
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    usernameField.stringValue = [defaults stringForKey:usernameKey];
    passwordField.stringValue = [defaults stringForKey:passwordKey];
}

#pragma mark Login

-(void)doLogin:(id)sender
{
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:usernameField.stringValue forKey:usernameKey];
    [defaults setObject:passwordField.stringValue forKey:passwordKey];

    ICEInitializationData* initData = [ICEInitializationData initializationData];
    initData.properties = [ICEUtil createProperties];
    [initData.properties load:[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"config.client"]];
    [initData.properties setProperty:@"Ice.ACM.Client.Timeout" value:@"0"];
    [initData.properties setProperty:@"Ice.RetryIntervals" value:@"-1"];

    initData.dispatcher = ^(id<ICEDispatcherCall> call, id<ICEConnection> con)
    {
        dispatch_sync(dispatch_get_main_queue(), ^ { [call run]; });
    };

    [initData.properties setProperty:@"IceSSL.DefaultDir" value:[[NSBundle mainBundle] resourcePath]];

    NSAssert(communicator == nil, @"communicator == nil");
    communicator = [ICEUtil createCommunicator:initData];

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
            id<GLACIER2RouterPrx> router = [GLACIER2RouterPrx checkedCast:[self.communicator getDefaultRouter]];;
            id<GLACIER2SessionPrx> glacier2session = [router createSession:username password:password];
            id<ChatChatSessionPrx> session = [ChatChatSessionPrx uncheckedCast:glacier2session];

            ICEInt acmTimeout = [router getACMTimeout];
            if(acmTimeout <= 0)
            {
                acmTimeout = (ICEInt)[router getSessionTimeout];
            }
            NSString* category = [router getCategoryForClient];

            ChatController* chatController = [[ChatController alloc] initWithCommunicator:self.communicator
                                                                                  session:session
                                                                               acmTimeout:acmTimeout
                                                                                   router:router
                                                                                 category:category];

            dispatch_async(dispatch_get_main_queue(), ^ {
                // Hide the connecting sheet.
                [NSApp endSheet:self.connectingSheet];
                [self.connectingSheet orderOut:self.window];
                [self.progress stopAnimation:self];

                // The communicator is now owned by the ChatController.
                self.communicator = nil;

                // Close the connecting window, show the main window.
                [self.window close];
                [chatController showWindow:self];
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
        @catch(ICEEndpointParseException* ex)
        {
            msg = [ex description];
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

@end
