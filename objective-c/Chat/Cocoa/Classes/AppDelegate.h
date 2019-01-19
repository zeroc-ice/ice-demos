//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ChatController;

@interface AppDelegate : NSObject
{
    NSWindowController* loginController;
    ChatController* chatController;
    BOOL terminate;
}

-(void)login:(id)sender;
-(void)setChatController:(ChatController*)controller;

@end
