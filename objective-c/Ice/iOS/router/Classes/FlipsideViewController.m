// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************


#import <FlipsideViewController.h>
#import <AppDelegate.h>

@implementation FlipsideViewController

@synthesize network;
@synthesize protocol;
@synthesize router;
@synthesize usePlatformCAs;
@synthesize checkCertName;

- (void)viewDidLoad {
    [super viewDidLoad];

    network.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:@"Ice.Trace.Network"];
    protocol.on = [[NSUserDefaults standardUserDefaults] integerForKey:@"Ice.Trace.Protocol"];
    router.on = [[NSUserDefaults standardUserDefaults] integerForKey:@"Trace.Router"];
    usePlatformCAs.on = [[NSUserDefaults standardUserDefaults] integerForKey:@"IceSSL.UsePlatformCAs"];
    checkCertName.on = [[NSUserDefaults standardUserDefaults] integerForKey:@"IceSSL.CheckCertName"];
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)networkChanged:(id)thesender
{
    UISegmentedControl* sender = thesender;
    NSInteger val = sender.selectedSegmentIndex;

    [[NSUserDefaults standardUserDefaults] setInteger:val forKey:@"Ice.Trace.Network"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)protocolChanged:(id)thesender
{
    UISwitch* sender = thesender;
    int val = (int)sender.isOn;
    [[NSUserDefaults standardUserDefaults] setInteger:val forKey:@"Ice.Trace.Protocol"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)routerChanged:(id)thesender
{
    UISwitch* sender = thesender;
    int val = (int)sender.isOn;
    [[NSUserDefaults standardUserDefaults] setInteger:val forKey:@"Trace.Router"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)platformCAsChanged:(id)thesender
{
    UISwitch* sender = thesender;
    int val = (int)sender.isOn;
    [[NSUserDefaults standardUserDefaults] setInteger:val forKey:@"IceSSL.UsePlatformCAs"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)checkCertNameChanged:(id)thesender {
    UISwitch* sender = thesender;
    int val = (int)sender.isOn;
    [[NSUserDefaults standardUserDefaults] setInteger:val forKey:@"IceSSL.CheckCertName"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

@end
