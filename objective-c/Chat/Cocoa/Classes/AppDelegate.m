// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <AppDelegate.h>
#import <LoginController.h>
#import <ChatController.h>

@implementation AppDelegate

-(id)init
{
    if(self = [super init])
    {
        loginController = [[LoginController alloc] init];
        terminate = NO;
    }
    return self;
}

-(void)applicationDidFinishLaunching:(NSNotification*)aNotification
{
    [loginController showWindow:self];
}

-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender
{
    if(chatController)
    {
        [chatController destroySession];
        terminate = YES;
        return NSTerminateLater; // Wait for the session to be destroyed to terminate.
    }
    else
    {
        return NSTerminateNow;
    }
}

-(void)login:(id)sender
{
    [loginController showWindow:self];
}

-(void)setChatController:(ChatController*)controller
{
    if(!controller && terminate)
    {
        [[NSApplication sharedApplication] replyToApplicationShouldTerminate:YES];
    }
    chatController = controller;
}

-(BOOL)validateMenuItem:(NSMenuItem *)item
{
    if([item action] == @selector(login:))
    {
        return !chatController && !loginController.window.isVisible;
    }
    return YES;
}

@end
