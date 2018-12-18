// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <Cocoa/Cocoa.h>
#import <ChatSession.h> // For ChatChatRoomCallback protocol
#import <objc/Ice.h>

@protocol GLACIER2RouterPrx;

@interface ChatController : NSWindowController<ChatChatRoomCallback>
{
@private
    IBOutlet NSTextView* chatView;
    IBOutlet NSTableView* userTable;
    IBOutlet NSTextField* inputField;

    id<ICECommunicator> communicator;
    id<ChatChatSessionPrx>  session;
    id<ChatChatRoomCallbackPrx> callbackProxy;
    int acmTimeout;
    id<GLACIER2RouterPrx> router;

    NSTimer* refreshTimer;

    NSMutableArray* users;

    NSDateFormatter* dateFormatter;

    NSDictionary *whoTextAttributes;
    NSDictionary *dateTextAttributes;
    NSDictionary *textAttributes;
}

-(id)initWithCommunicator:(id<ICECommunicator>)communicator
                  session:(id<ChatChatSessionPrx>)session
               acmTimeout:(int)acmTiemout
                   router:(id<ICERouterPrx>)router
                 category:(NSString*)category;
-(void)destroySession;
-(IBAction)sendChat:(id)sender;
-(void)logout:(id)sender;

@end
