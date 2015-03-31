// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
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
