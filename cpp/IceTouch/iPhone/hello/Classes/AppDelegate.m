// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
//
// **********************************************************************

#import <AppDelegate.h>
#import <HelloController.h>

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
    // Override point for customization after app launch	
    [window setRootViewController:viewController];
    [window makeKeyAndVisible];
}

- (void)dealloc
{
    [viewController release];
    [window release];
    [super dealloc];
}

@end
