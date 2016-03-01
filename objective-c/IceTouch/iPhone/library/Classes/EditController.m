// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <EditController.h>

@implementation EditController

@synthesize textField;

-(void)viewDidLoad
{
    // Adjust the text field size and font.
    CGRect frame = textField.frame;
    frame.size.height += 10;
    textField.frame = frame;
    textField.font = [UIFont boldSystemFontOfSize:16];
    // Set the view background to match the grouped tables in the other views.
    self.view.backgroundColor = [UIColor groupTableViewBackgroundColor];
}

-(void)viewWillAppear:(BOOL)animated
{
    textField.placeholder = self.title;
    textField.enabled = YES;
    textField.text = value;
    
    [textField becomeFirstResponder];
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

-(void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}

-(IBAction)cancel:(id)sender
{
    [self.navigationController popViewControllerAnimated:YES];
}

-(void)startEdit:(id)o selector:(SEL)sel name:(NSString*)name value:(NSString*)v
{
    obj = o;
    selector = sel;
    
    self.title = name;
    value = v;
}

-(IBAction)save:(id)sender
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
    [obj performSelector:selector withObject:textField.text];
#pragma clang diagnostic pop    
    [self.navigationController popViewControllerAnimated:YES];
}

@end
