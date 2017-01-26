// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************


#import <AppDelegate.h>
#import <RootViewController.h>
#import <RouterI.h>
#import <Router.h>
#import <LoggingDelegate.h>

@implementation Logger

@synthesize delegate;

-(void) print:(NSString*)message
{
    if(delegate)
    {
        [delegate performSelectorOnMainThread:@selector(log:)
                                   withObject:[LogEntry logEntryPrint:message]
                                waitUntilDone:NO];
    }
}

-(void) trace:(NSString*)category message:(NSString*)message
{
    if(delegate)
    {
        [delegate performSelectorOnMainThread:@selector(log:)
                                   withObject:[LogEntry logEntryTrace:message category:category]
                                waitUntilDone:NO];
    }
}

-(void) warning:(NSString*)message
{
    if(delegate)
    {
        [delegate performSelectorOnMainThread:@selector(log:)
                                   withObject:[LogEntry logEntryWarning:message]
                                waitUntilDone:NO];
    }
}

-(void) error:(NSString*)message
{
    if(delegate)
    {
        [delegate performSelectorOnMainThread:@selector(log:)
                                   withObject:[LogEntry logEntryError:message]
                                waitUntilDone:NO];
    }
}

-(NSMutableString*) getPrefix
{
    return [NSMutableString string];
}

-(id<ICELogger>) cloneWithPrefix:(NSString *)prefix
{
    return self;
}

@end

@implementation AppDelegate

@synthesize window;
@synthesize rootViewController;
@synthesize communicator;
@synthesize router;
@synthesize logger;

-(id)init
{
    if (self = [super init])
    {
        NSDictionary* appDefaults = [NSDictionary dictionaryWithObjectsAndKeys:
                                     @"0", @"Ice.Trace.Network",
                                     @"0", @"Ice.Trace.Protocol",
                                     @"1", @"Trace.Router",
                                     @"1", @"IceSSL.UsePlatformCAs",
                                     @"1", @"IceSSL.CheckCertName",
                                     nil];

        [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
        [[NSUserDefaults standardUserDefaults] synchronize];

        ICEInitializationData* initData = [ICEInitializationData initializationData];
        initData.properties = [ICEUtil createProperties];

        [initData.properties setProperty:@"Client.Endpoints" value:@"tcp -p 12000"];
        [initData.properties setProperty:@"Server.Endpoints" value:@"tcp"];

        [initData.properties setProperty:@"RoutedServer.Endpoints" value:@""];

        [initData.properties setProperty:@"Ice.Trace.Network" value:[[NSUserDefaults standardUserDefaults] stringForKey:@"Ice.Trace.Network"]];
        [initData.properties setProperty:@"Ice.Trace.Protocol" value:[[NSUserDefaults standardUserDefaults] stringForKey:@"Ice.Trace.Protocol"]];

        [initData.properties setProperty:@"Ice.ThreadPool.Server.SizeMax" value:@"10"];

        [initData.properties setProperty:@"Trace.Router" value:[[NSUserDefaults standardUserDefaults] stringForKey:@"Trace.Router"]];

        [initData.properties setProperty:@"IceSSL.UsePlatformCAs" value:[[NSUserDefaults standardUserDefaults] stringForKey:@"IceSSL.UsePlatformCAs"]];
        [initData.properties setProperty:@"IceSSL.CheckCertName" value:[[NSUserDefaults standardUserDefaults] stringForKey:@"IceSSL.CheckCertName"]];

        self.logger = [[Logger alloc] init];
        initData.logger = logger;
        self.communicator = [ICEUtil createCommunicator:initData];
    }
    return self;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {

    [window setRootViewController:rootViewController];
    [window makeKeyAndVisible];
}

-(void)initializeRouter
{
    @try
    {
        id<ICEObjectAdapter> clientAdapter = [communicator createObjectAdapter:@"Client"];

        self.router = [[RouterI alloc] initWithCommunicator:communicator];
        [clientAdapter add:self.router identity:[ICEUtil stringToIdentity:@"iPhoneRouter/Router"]];
        [clientAdapter addDefaultServant:router.clientBlobject category:@""];
        [clientAdapter activate];
    }
    @catch(ICELocalException* ex)
    {
        NSMutableString* s = [NSMutableString stringWithFormat:@"failed to initialize communicator:\n %@", ex];
        [s replaceOccurrencesOfString:@"\n" withString:@" " options:0 range:NSMakeRange(0, s.length)];
        NSLog(@"%@", s);
    }
}


@end
