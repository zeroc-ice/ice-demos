// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
//
// **********************************************************************


#import <UIKit/UIKit.h>
#import <LoggingDelegate.h>

@interface FlipsideViewController : UIViewController
{
    UISegmentedControl* network;
    UISwitch* protocol;
    UISwitch* router;
}

@property (nonatomic) IBOutlet UISegmentedControl* network;
@property (nonatomic) IBOutlet UISwitch* protocol;
@property (nonatomic) IBOutlet UISwitch* router;

-(void)networkChanged:(id)sender;
-(void)protocolChanged:(id)sender;
-(void)routerChanged:(id)sender;
@end
