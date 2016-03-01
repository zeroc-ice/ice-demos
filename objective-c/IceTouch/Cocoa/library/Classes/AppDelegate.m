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
        connectController = [[LoginController alloc] init];
    }
    return self;
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [connectController showWindow:self];
}

-(void)login:(id)sender
{
    [connectController showWindow:self];
}

-(void)setLibraryActive:(BOOL)active
{
    chatActive = active;
}

- (BOOL)validateMenuItem:(NSMenuItem *)item
{
    if ([item action] == @selector(login:))
    {
        return !chatActive && !connectController.window.isVisible;
    }
    return YES;
}

@end
