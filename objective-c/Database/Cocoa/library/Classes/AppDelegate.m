// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <AppDelegate.h>
#import <LoginController.h>
#import <LibraryController.h>

@implementation AppDelegate

-(id)init
{
    if(self = [super init])
    {
        connectController = [[LoginController alloc] init];
        terminate = NO;
    }
    return self;
}

-(void)applicationDidFinishLaunching:(NSNotification*)aNotification
{
    [connectController showWindow:self];
}

-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender
{
    if(libraryController)
    {
        [libraryController destroySession];
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
    [connectController showWindow:self];
}

-(void)setLibraryController:(LibraryController*)controller
{
    if(!controller && terminate)
    {
        [[NSApplication sharedApplication] replyToApplicationShouldTerminate:YES];
    }
    libraryController = controller;
}

- (BOOL)validateMenuItem:(NSMenuItem *)item
{
    if ([item action] == @selector(login:))
    {
        return !libraryController && !connectController.window.isVisible;
    }
    return YES;
}

@end
