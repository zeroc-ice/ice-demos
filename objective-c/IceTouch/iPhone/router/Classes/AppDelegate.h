// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************


#import <UIKit/UIKit.h>
#import <objc/Ice.h>

@class RouterI;
@class RootViewController;
@protocol LoggingDelegate;

@interface Logger : NSObject<ICELogger>
{
    NSObject<LoggingDelegate>* delegate;
}

@property NSObject<LoggingDelegate>* delegate;

@end

@interface AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    RootViewController *rootViewController;
    id<ICECommunicator> communicator;
    RouterI* router;
    Logger* logger;
}

@property (nonatomic) IBOutlet UIWindow *window;
@property (nonatomic) IBOutlet RootViewController *rootViewController;
@property (nonatomic) id<ICECommunicator> communicator;
@property (nonatomic) RouterI* router;
@property (nonatomic) Logger* logger;

-(void)initializeRouter;

@end

