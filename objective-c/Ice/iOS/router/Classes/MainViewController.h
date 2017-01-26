// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>
#import <RouterDelegate.h>

@class LogViewController;

@interface MainViewController : UIViewController<RouterDelegate> {
    IBOutlet UITextField* statusTextField;
    IBOutlet UITextField* clientRequestsTextField;
    IBOutlet UITextField* clientExceptionsTextField;
    IBOutlet UITextField* serverRequestsTextField;
    IBOutlet UITextField* serverExceptionsTextField;
}

@end
