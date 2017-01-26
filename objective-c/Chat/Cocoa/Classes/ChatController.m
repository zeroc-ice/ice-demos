// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <ChatController.h>
#import <AppDelegate.h>

#import <objc/Ice.h>
#import <objc/Glacier2.h>

@implementation ChatController

-(void)closed:(id<ICEConnection>)connection
{
    // The session is invalid, clear.
    if(session != nil)
    {
        [self destroySession];
        
        NSRunAlertPanel(@"Error", @"%@", @"OK", nil, nil, @"Lost connection with session!\n");
    }
}

-(void)heartbeat:(id<ICEConnection>)connection
{
}

// This is called outside of the main thread.
-(id)initWithCommunicator:(id<ICECommunicator>)c
                  session:(id<ChatChatSessionPrx>)s
               acmTiemout:(int)t
                   router:(id<GLACIER2RouterPrx>)r
                 category:(NSString*)category
{ 
    if(self = [super initWithWindowNibName:@"ChatView"])
    {
        communicator = c;
        session = s;
        router = r;
        acmTimeout = t;
        
        // Set up the adapter, and register the callback object, and setup the session ping.
        id<ICEObjectAdapter> adapter = [communicator createObjectAdapterWithRouter:@"ChatDemo.Client" router:router];
        [adapter activate];
		
        ICEIdentity* callbackId = [ICEIdentity identity:[ICEUtil generateUUID] category:category];
        
        // Here we tie the chat view controller to the ChatRoomCallback servant.
        ChatChatRoomCallback* callbackImpl = [ChatChatRoomCallback objectWithDelegate:self];
        
        id<ICEObjectPrx> proxy = [adapter add:callbackImpl identity:callbackId];
		
        // The callback is registered in awakeFromNib, otherwise the callbacks can arrive
        // prior to the IBOutlet connections being setup.
        callbackProxy = [ChatChatRoomCallbackPrx uncheckedCast:proxy];
		
        users = [NSMutableArray array];
		
        dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateStyle:NSDateFormatterNoStyle];
        [dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
        
        // Cached attributes for the text view.
        whoTextAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
                             [NSColor blueColor],
                             NSForegroundColorAttributeName,
                             nil];
        
        dateTextAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSColor blackColor],
                              NSForegroundColorAttributeName,
                              nil];
        
        textAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSColor lightGrayColor],
                          NSForegroundColorAttributeName,
                          nil];
    }
	
    return self; 
}

-(void)append:(NSString*)text who:(NSString*)who timestamp:(NSDate*)timestamp
{
    // De-HTMLize the incoming message.
    NSMutableString* s = [text mutableCopy];
    NSString* replace[] =
    {
        @"&quot;",
        @"\"",
        @"&#39;", @"'",
        @"&lt;", @"<",
        @"&gt;", @">",
        @"&amp;", @"&"
    };
    int i;
    for(i = 0; i < sizeof(replace)/sizeof(replace[0]); i += 2)
    {
        [s replaceOccurrencesOfString:replace[i]
                           withString:replace[i+1] options:NSCaseInsensitiveSearch
                                range:NSMakeRange(0, s.length)];
    }
    
    text = s;
    
    [chatView.textStorage appendAttributedString:[[NSAttributedString alloc]
                                                  initWithString:[dateFormatter stringFromDate:timestamp]
                                                  attributes:dateTextAttributes]];
    [chatView.textStorage appendAttributedString:[[NSAttributedString alloc]
                                                  initWithString:@" - "]];
    [chatView.textStorage appendAttributedString:[[NSAttributedString alloc]
                                                  initWithString:who
                                                  attributes:whoTextAttributes]];
    [chatView.textStorage appendAttributedString:[[NSAttributedString alloc]
                                                  initWithString:@" - "]];
    [chatView.textStorage appendAttributedString:[[NSAttributedString alloc]
                                                  initWithString:text
                                                  attributes:textAttributes]];
    [chatView.textStorage appendAttributedString:[[NSAttributedString alloc]
                                                  initWithString:@"\n"]];
    
    // Scroll the chatView to the end.
    [chatView scrollRangeToVisible:NSMakeRange(chatView.string.length, 0)];
}

-(void)destroySession
{
    // Destroy the session.
    if(router && [router isKindOfClass:[GLACIER2RouterPrx class]])
    {
        id<GLACIER2RouterPrx> glacier2router = [GLACIER2RouterPrx uncheckedCast:router];
        [glacier2router begin_destroySession];
    }
    else
    {
        [session begin_destroy];
    }
    router = nil;
    session = nil;
    
    // Clean up the communicator.
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
            // Destroy might block so we call it from a separate thread.
            [communicator destroy];
            communicator = nil;
            
            dispatch_async(dispatch_get_main_queue(), ^ {
                    [self append:@"<disconnected>" who:@"system message" timestamp:[NSDate date]];
                    [inputField setEnabled:NO];
                    
                    NSApplication* app = [NSApplication sharedApplication];
                    AppDelegate* delegate = (AppDelegate*)[app delegate];
                    [delegate setChatActive:NO];
            });
    });
}

-(void)exception:(ICEException*)ex
{
    [self destroySession];
        
    NSRunAlertPanel(@"Error", @"%@", @"OK", nil, nil, [ex description]);
}

-(void)awakeFromNib
{
    NSApplication* app = [NSApplication sharedApplication];
    AppDelegate* delegate = (AppDelegate*)[app delegate];
    [delegate setChatActive:YES];
    
    id<ICEConnection> conn = [router ice_getCachedConnection];
    id heartbeat = @(ICEHeartbeatAlways);
    id timeout = [NSNumber numberWithInteger:acmTimeout];
    [conn setACM:timeout close:ICENone heartbeat:heartbeat];
    [conn setCloseCallback:^(id<ICEConnection> connection) {
        [self closed:connection];
    }];
    
    [chatView.textStorage deleteCharactersInRange:NSMakeRange(0, chatView.textStorage.length)];
    
    // Register the chat callback.
    [session begin_setCallback:callbackProxy response:nil exception:^(ICEException* ex) { [self exception:ex]; }];
}

- (BOOL)validateMenuItem:(NSMenuItem *)item
{
    if ([item action] == @selector(logout:))
    {
        return session != nil;
    }
    return YES;
}

-(void)windowWillClose:(NSNotification *)notification
{
    [self destroySession];
}

-(void)logout:(id)sender
{
    [self destroySession];
}

-(IBAction)sendChat:(id)sender
{
    NSString* s  = [inputField.stringValue stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    if(s.length > 1024)
    {
        s = [s substringToIndex:1024];
    }
    
    NSAssert(s.length <= 1024, @"s.length <= 1024");
    if(s.length > 0)
    {
        [session begin_send:s response:^(ICELong t) { } exception:^(ICEException* ex) { [self exception:ex]; }];
    }
	
    inputField.stringValue = @"";
}

#pragma mark ChatRoomCallbck

-(void)init:(NSMutableArray *)u current:(ICECurrent*)current;
{
    users = u;
    [userTable reloadData];
}

-(void)send:(ICELong)timestamp name:(NSMutableString *)name message:(NSMutableString *)message current:(ICECurrent*)current;
{
    [self append:message
             who:name
       timestamp:[NSDate dateWithTimeIntervalSinceReferenceDate:(timestamp/ 1000.f) - NSTimeIntervalSince1970]];
}

-(void)join:(ICELong)timestamp name:(NSMutableString*)name current:(ICECurrent*)current;
{
    [users addObject:name];
    [userTable reloadData];
	
    [self append:[NSString stringWithFormat:@"%@ joined.", name]
             who:@"system message"
       timestamp:[NSDate dateWithTimeIntervalSinceReferenceDate:(timestamp/ 1000.f) - NSTimeIntervalSince1970]];
}

-(void)leave:(ICELong)timestamp name:(NSMutableString*)name current:(ICECurrent*)current;
{
    NSUInteger index = [users indexOfObject:name];
    if(index != NSNotFound)
    {
        [users removeObjectAtIndex:index];
        [userTable reloadData];
    }
	
    [self append:[NSString stringWithFormat:@"%@ left.", name]
             who:@"system message"
       timestamp:[NSDate dateWithTimeIntervalSinceReferenceDate:(timestamp/ 1000.f) - NSTimeIntervalSince1970]];
}

#pragma mark NSTableView delegate

- (int)numberOfRowsInTableView:(NSTableView *)tv 
{ 
    return users.count;
}

-(id) tableView:(NSTableView *)tv 
objectValueForTableColumn:(NSTableColumn *)tableColumn 
            row:(int)row 
{ 
    return [users objectAtIndex:row];
}

@end
