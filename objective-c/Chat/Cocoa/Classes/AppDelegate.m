// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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
