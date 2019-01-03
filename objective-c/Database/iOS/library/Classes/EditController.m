// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <EditController.h>

@implementation EditController

@synthesize textField;
@synthesize initialValue;

-(void)viewDidLoad
{
    textField.placeholder = self.title;
    textField.text = initialValue;
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

-(void)startEdit:(NSString*)name value:(NSString*)v
{
    self.title = name;
    self.initialValue = v;
}

@end
