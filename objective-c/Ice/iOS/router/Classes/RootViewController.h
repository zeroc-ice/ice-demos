// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>

@class MainViewController;
@class FlipsideViewController;
@class LogViewController;
@class LogZoomViewController;

@interface RootViewController : UIViewController {

    UIButton *infoButton;
    UIButton *logButton;
    MainViewController *mainViewController;

    FlipsideViewController *flipsideViewController;
    UINavigationBar *flipsideNavigationBar;
    
    UINavigationBar *logNavigationBar;
    LogViewController *logViewController;

    UINavigationBar *logZoomNavigationBar;
    LogZoomViewController *logZoomViewController;
}

@property (nonatomic) IBOutlet UIButton *infoButton;
@property (nonatomic) IBOutlet UIButton *logButton;

@property (nonatomic) MainViewController *mainViewController;

@property (nonatomic) UINavigationBar *flipsideNavigationBar;
@property (nonatomic) FlipsideViewController *flipsideViewController;

@property (nonatomic) UINavigationBar *logNavigationBar;
@property (nonatomic) LogViewController *logViewController;

@property (nonatomic) UINavigationBar *logZoomNavigationBar;
@property (nonatomic) LogZoomViewController *logZoomViewController;

- (IBAction)toggleView;
- (IBAction)toggleLogView;
- (IBAction)toggleLogZoomView;

@end
