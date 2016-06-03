// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************


#import <MainViewController.h>
#import <LogViewController.h>
#import <MainView.h>
#import <AppDelegate.h>
#import <RouterI.h>

@implementation MainViewController


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    AppDelegate* app = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    [app initializeRouter];
    [app.router setDelegate:self];
    [self refreshRoutingStatistics];
    [super viewDidLoad];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

-(void)refreshRoutingStatistics
{
    AppDelegate* app = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    [statusTextField setText:app.router.status];
    [clientRequestsTextField setText:[NSString stringWithFormat:@"%d", app.router.clientRequests]];
    [clientExceptionsTextField setText:[NSString stringWithFormat:@"%d", app.router.clientExceptions]];
    [serverRequestsTextField setText:[NSString stringWithFormat:@"%d", app.router.serverRequests]];
    [serverExceptionsTextField setText:[NSString stringWithFormat:@"%d", app.router.serverExceptions]];
}

@end
