// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
//
// **********************************************************************

#import <AppDelegate.h>
#import <LoginController.h>

@implementation AppDelegate

-(id)init
{
    if(self = [super init])
    {
        loginController = [[LoginController alloc] init];
    }
    return self;
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [loginController showWindow:self];
}

-(void)login:(id)sender
{
    [loginController showWindow:self];
}

-(void)setChatActive:(BOOL)active
{
    chatActive = active;
}

- (BOOL)validateMenuItem:(NSMenuItem *)item
{
    if ([item action] == @selector(login:))
    {
        return !chatActive && !loginController.window.isVisible;
    }
    return YES;
}

@end
