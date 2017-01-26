// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>

@class VoipViewController;

@interface AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    VoipViewController *viewController;
}

@property (nonatomic) IBOutlet UIWindow *window;
@property (nonatomic) IBOutlet VoipViewController *viewController;

@end

