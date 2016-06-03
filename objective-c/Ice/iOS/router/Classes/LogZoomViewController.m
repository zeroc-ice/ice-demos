// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <LogZoomViewController.h>
#import <LoggingDelegate.h>

@implementation LogZoomViewController

@synthesize text;
@synthesize messages;
@synthesize current;
@synthesize nextButton;
@synthesize prevButton;
@synthesize dateFormatter;


// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        self.dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateStyle:NSDateFormatterShortStyle];
        [dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
    }
    return self;
}

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

-(void)refresh
{
    LogEntry* entry = [messages objectAtIndex:current];

    NSMutableString* s = [NSMutableString stringWithCapacity:0];
    [s appendFormat:@"Date: %@", [dateFormatter stringFromDate:entry.timestamp]];
    switch(entry.type)
    {
        case LogEntryTypeWarning:
            [s appendString:@"\nType: Warning\n"];
            break;

        case LogEntryTypeError:
            [s appendString:@"\nType: Error\n"];
            break;

        case LogEntryTypePrint:
            [s appendString:@"\nType: Print\n"];
            break;

        case LogEntryTypeTrace:
            [s appendFormat:@"\nType: Trace\nCategory: %@\n", entry.category];
            break;
    }
    [s appendString:@"Message:\n"];

    [s appendString:entry.message];
    
    text.text = s;

    [prevButton setEnabled:(current != 0)];
    [nextButton setEnabled:(current != messages.count-1)];
}
-(void)viewWillAppear:(BOOL)animated
{
    [self refresh];
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

-(IBAction)next:(id)sender
{
    current += 1;
    [self refresh];
}

-(IBAction)prev:(id)sender
{
    current -= 1;
    [self refresh];
}

@end
