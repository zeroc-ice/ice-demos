// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>

@class HelloController;

@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    IBOutlet UIWindow *window;
    IBOutlet HelloController *viewController;
}

@end
