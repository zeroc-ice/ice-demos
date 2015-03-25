// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
//
// **********************************************************************

#import <Cocoa/Cocoa.h>
#import <ChatSession.h> // For ChatChatRoomCallback protocol

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
    int sessionTimeout;
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
           sessionTimeout:(int)sessionTimeout
                   router:(id<ICERouterPrx>)router
                 category:(NSString*)category;

-(IBAction)sendChat:(id)sender;
-(void)logout:(id)sender;

@end
