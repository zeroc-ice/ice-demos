// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
//
// **********************************************************************

#import <LoginController.h>
#import <LibraryController.h>

#import <Library.h>
#import <Glacier2Session.h>
#import <Session.h>
#import <Router.h>

#import <objc/Ice.h>

NSString* const serverKey = @"hostnameKey";
NSString* const usernameKey = @"usernameKey";
NSString* const passwordKey = @"passwordKey";
NSString* const sslKey = @"sslKey";
NSString* const glacier2Key = @"glacier2Key";
NSString* const routerKey = @"routerKey";
NSString* const routerServerKey = @"routerServerKey";

@implementation LoginController

+(void)initialize
{
    // Initialize the application defaults.
    NSDictionary* appDefaults = [NSDictionary dictionaryWithObjectsAndKeys:
                                 @"demo2.zeroc.com", serverKey,
                                 @"", usernameKey,
                                 @"", passwordKey,
                                 @"YES", sslKey,
                                 @"YES", glacier2Key,
                                 @"NO", routerKey,
                                 @"", routerServerKey,
                                 nil];
    
    [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
    
}

- (id)init
{ 
    return [super initWithWindowNibName:@"LoginView"];
}

-(void)awakeFromNib
{
    // Initialize the fields from the application defaults.
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    
    chatServerField.stringValue = [defaults stringForKey:serverKey];
    usernameField.stringValue = [defaults stringForKey:usernameKey];
    passwordField.stringValue = [defaults stringForKey:passwordKey];
    sslField.state = [defaults boolForKey:sslKey] ? NSOnState : NSOffState;
    glacier2Field.state = [defaults boolForKey:glacier2Key] ? NSOnState : NSOffState;
    routerField.state = [defaults boolForKey:routerKey] ? NSOnState : NSOffState;
    routerServerField.stringValue = [defaults stringForKey:routerServerKey];
    [routerServerField setEnabled:routerField.state == NSOnState];
    if(routerField.state == NSOnState)
    {
        [sslField setEnabled:routerField.state != NSOnState];
        sslField.state = NSOffState;
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
    id<GLACIER2RouterPrx> router = [GLACIER2RouterPrx checkedCast:proxy];
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
    ICEInt sessionTimeout;
    
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
	
    if(sslField.state == NSOnState && routerField.state != NSOnState)
    {   
        [[initData properties] setProperty:@"IceSSL.TrustOnly.Client"
                                     value:@"CN=\"127.0.0.1\""];
        [[initData properties] setProperty:@"IceSSL.CertAuthFile" value:@"cacert.pem"];
    }
    [initData.properties setProperty:@"IceSSL.DefaultDir" value:[[NSBundle mainBundle] resourcePath]];
    
    NSAssert(communicator == nil, @"communicator == nil");
    communicator = [ICEUtil createCommunicator:initData];
	
    SEL loginSelector;
    id<ICEObjectPrx> proxy;
    @try
    {
        if(routerField.state == NSOnState)
        {
            NSString* s;
            if(sslField.state == NSOnState)
            {
                s = [NSString stringWithFormat:@"iPhoneRouter/Router:ssl -p 12001 -h %@ -t 10000",
                     routerServerField.stringValue];
            }
            else
            {
                s = [NSString stringWithFormat:@"iPhoneRouter/Router:tcp -p 12000 -h %@ -t 10000",
                     routerServerField.stringValue];
            }
            proxy = [communicator stringToProxy:s];
            id<ICERouterPrx> router = [ICERouterPrx uncheckedCast:proxy];
            [communicator setDefaultRouter:router];
			
            if(glacier2Field.state == NSOnState)
            {
                // The proxy to the Glacier2 router.
                proxy = [communicator stringToProxy:
                         [NSString stringWithFormat:@"DemoGlacier2/router:tcp -p 4063 -h %@ -t 10000",
                          chatServerField.stringValue]];
                loginSelector = @selector(doPhoneRouterGlacier2Login:);
            }
            else
            {
                // The proxy to the library session server.
                proxy = [communicator stringToProxy:
                         [NSString stringWithFormat:@"SessionFactory:tcp -h %@ -p 10000 -t 10000",
                          chatServerField.stringValue]];
                loginSelector = @selector(doPhoneRouterLogin:);
            }
        }
        else
        {
            if(glacier2Field.state == NSOnState)
            {
                NSString* s;
                if(sslField.state == NSOnState)
                {
                    s = [NSString stringWithFormat:@"DemoGlacier2/router:ssl -p 4064 -h %@ -t 10000",
                         chatServerField.stringValue];
                }
                else
                {
                    s = [NSString stringWithFormat:@"DemoGlacier2/router:tcp -p 4063 -h %@ -t 10000",
                         chatServerField.stringValue];
                }
				
                proxy = [communicator stringToProxy:s];
                id<ICERouterPrx> router = [ICERouterPrx uncheckedCast:proxy];
                [communicator setDefaultRouter:router];
                loginSelector = @selector(doGlacier2Login:);
            }
            else
            {
                NSString* s;
                if(sslField.state == NSOnState)
                {
                    s = [NSString stringWithFormat:@"SessionFactory:ssl -p 10001 -h %@ -t 10000",
                         chatServerField.stringValue];
                }
                else
                {
                    s = [NSString stringWithFormat:@"SessionFactory:tcp -h %@ -p 10000 -t 10000",
                         chatServerField.stringValue];
                }
                proxy = [communicator stringToProxy:s];
                loginSelector = @selector(doLogin:);
            }
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

-(void)routerChanged:(id)sender
{
    BOOL on = routerField.state == NSOnState;
    [routerServerField setEnabled:on];
    if(on)
    {
        sslField.state = NSOffState;
    }
    [sslField setEnabled:!on];
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
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    
    [defaults setObject:chatServerField.stringValue forKey:serverKey];
    [defaults setBool:(sslField.state == NSOnState) forKey:sslKey];
    [defaults setBool:(glacier2Field.state == NSOnState) forKey:glacier2Key];
    [defaults setBool:(routerField.state == NSOnState) forKey:routerKey];
    [defaults setObject:routerServerField.stringValue forKey:routerServerKey];
	
    [NSApp endSheet:advancedSheet]; 
    [advancedSheet orderOut:sender]; 
}

@end
