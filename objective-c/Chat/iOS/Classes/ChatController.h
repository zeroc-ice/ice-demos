// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>
#import <ChatSession.h>
#import <objc/Ice.h>

@protocol GLACIER2RouterPrx;
@class UserController;

@interface ChatController : UIViewController<UITableViewDelegate, UITableViewDataSource, UITextFieldDelegate,
                                             ChatChatRoomCallback>
{
@private
    IBOutlet UITableView* chatView;
    IBOutlet UITextField* inputField;
    NSMutableArray* users;
    NSMutableArray* messages;
    NSTimer* refreshTimer;

    id<ICECommunicator> communicator;
    id<ChatChatSessionPrx>  session;
    id<GLACIER2RouterPrx> router;
    id<ChatChatRoomCallbackPrx> callbackProxy;
    ICEInt acmTimeout;
}

// Called to setup the session.
-(void)  setup:(id<ICECommunicator>)communicator
       session:(id<ChatChatSessionPrx>)session
    acmTimeout:(ICEInt)timeout
        router:(id<GLACIER2RouterPrx>)router
      category:(NSString*)category;
// Called just prior to being shown.
-(void)activate;

// ChatChatRoomCallback methods.
-(void)init:(NSMutableArray *)users current:(ICECurrent*)current;
-(void)send:(ICELong)timestamp name:(NSMutableString *)name message:(NSMutableString *)message
    current:(ICECurrent*)current;
-(void)join:(ICELong)timestamp name:(NSMutableString *)name current:(ICECurrent*)current;
-(void)leave:(ICELong)timestamp name:(NSMutableString *)name current:(ICECurrent*)current;

-(IBAction)logout:(id)sender;
@end
