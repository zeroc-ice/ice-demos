// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <AddController.h>
#import <Library.h>

@interface AddController ()

@property (nonatomic) id<DemoLibraryPrx> library;

@end

@implementation AddController

@synthesize library;

-(void)viewDidLoad
{
    self.title = @"New Book";
    tableView.allowsSelectionDuringEditing = YES;
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.navigationItem.leftBarButtonItem.enabled = YES;
    self.navigationItem.rightBarButtonItem.enabled = (book.isbn && book.isbn.length > 0);
    [self setEditing:YES animated:NO];
    [self.navigationItem setHidesBackButton:NO animated:animated];
}

-(void)didReceiveMemoryWarning
{
  [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
  // Release anything that's not essential, such as cached data
}

-(IBAction)save:(id)sender
{
    [self saving:YES];
    [library begin_createBook:book.isbn
                        title:book.title
                      authors:book.authors
                     response:^(id<DemoBookPrx> prx) {
                         [self saving:NO];
             [self.navigationController popViewControllerAnimated:YES];
           }
                    exception:^(ICEException* ex) {
            if([ex isKindOfClass:[DemoBookExistsException class]])
            {
                            [self saving:NO];
              // open an alert with just an OK button
              UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Error"
                                                                             message:@"That ISBN number already exists"
                                                                      preferredStyle:UIAlertControllerStyleAlert];
              [alert addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];
              [self presentViewController:alert animated:YES completion:nil];
              return;
            }

            [super exception:ex];
          }];
}

-(void)startEdit:(DemoBookDescription*)b library:(id<DemoLibraryPrx>)l
{
    self.book = b;
    self.library = l;
    [self setEditing:YES animated:NO];
}

@end
