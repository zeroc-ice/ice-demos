// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************


#import <UIKit/UIKit.h>
#import <LoggingDelegate.h>

@interface FlipsideViewController : UIViewController
{
    // tracing
    UISegmentedControl* network;
    UISwitch* protocol;
    UISwitch* router;
    // ssl
    UISwitch* usePlatformCAs;
    UISwitch* checkCertName;
}

@property (nonatomic) IBOutlet UISegmentedControl* network;
@property (nonatomic) IBOutlet UISwitch* protocol;
@property (nonatomic) IBOutlet UISwitch* router;
@property (nonatomic) IBOutlet UISwitch* usePlatformCAs;
@property (nonatomic) IBOutlet UISwitch* checkCertName;

-(void)networkChanged:(id)sender;
-(void)protocolChanged:(id)sender;
-(void)routerChanged:(id)sender;
-(void)platformCAsChanged:(id)sender;
-(void)checkCertNameChanged:(id)sender;

@end
