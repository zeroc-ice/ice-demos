// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>

@protocol ICECommunicator;
@class ICEException;
@class ICEInitializationData;

@interface AppDelegate : NSObject <UIApplicationDelegate>
{
@private
    
    IBOutlet UIWindow *window;
    IBOutlet UINavigationController *navigationController;

}

@end

