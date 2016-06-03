// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <MainController.h>
#import <DetailController.h>
#import <AddController.h>

#import <Session.h>
#import <Library.h>

#import <objc/Ice.h>
#import <objc/Glacier2.h>

@interface MainController()

@property (nonatomic) NSIndexPath* currentIndexPath;
@property (nonatomic) id<DemoBookQueryResultPrx> query;

@property (nonatomic) NSTimer* refreshTimer;
@property (nonatomic) id<ICECommunicator> communicator;
@property (nonatomic) id session;
@property (nonatomic)  id<GLACIER2RouterPrx> router;
@property (nonatomic) id<DemoLibraryPrx> library;

-(void)exception:(ICEException*)ex;

@end

@implementation MainController

@synthesize library;
@synthesize query;
@synthesize currentIndexPath;
@synthesize communicator;
@synthesize refreshTimer;
@synthesize session;
@synthesize router;

-(id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])
    {
        self.title = @"Search"; // Hostname?

		// Initialization code
        books = [NSMutableArray array];
        nrows = 0;
        rowsQueried = 0;
        
        detailController = [[DetailController alloc] initWithNibName:@"DetailView" bundle:nil];
        detailController.delegate = self;

        addController = [[AddController alloc] initWithNibName:@"DetailView" bundle:nil];
	}
	return self;
}

-(void)viewDidLoad
{
    self.navigationItem.rightBarButtonItem =
    [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd
                                                   target:self
                                                   action:@selector(addBook:)];
    self.navigationItem.leftBarButtonItem =
    [[UIBarButtonItem alloc] initWithTitle:@"Logout"
                                     style:UIBarButtonItemStylePlain
                                     target:self action:@selector(logout:)];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(destroySession)
                                                 name:UIApplicationWillTerminateNotification
                                               object:nil];    
}

-(void)viewWillAppear:(BOOL)animated
{
    // There was a fatal error and the session was destroyed.
    if(session == nil)
    {
        [self.navigationController popViewControllerAnimated:YES];
    }
    // Redisplay the data.
    [searchTableView reloadData];
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


#pragma mark SessionManagement

-(void)activate:(id<ICECommunicator>)c
        session:(id)s
         router:(id<GLACIER2RouterPrx>)r
 sessionTimeout:(ICELong)sessionTimeout
        library:(id<DemoLibraryPrx>)l
{
    self.communicator = c;
    self.session = s;
    self.router = r;
    self.library = l;
    self.query = nil;
    nrows = 0;
    rowsQueried = 10;
    [books removeAllObjects];
    
    // Save the new session, and create the refresh timer.
    self.refreshTimer = [NSTimer
                         timerWithTimeInterval:sessionTimeout/2
                         target:self
                         selector:@selector(refreshSession:)
                         userInfo:nil
                         repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:refreshTimer forMode:NSDefaultRunLoopMode];
}

-(void)destroySession
{
    // Destroy the old session, and invalidate the refresh timer.
    [refreshTimer invalidate];
    self.refreshTimer = nil;
    
    if(router)
    {
        [router begin_destroySession];
    }
    else
    {
        [session begin_destroy];
    }
    router = nil;
    session = nil;
    
	// Destroy the communicator from another thread since this call blocks.
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        @try
        {            
            [communicator destroy];
            communicator = nil;
        }
        @catch (ICEException* ex) {
        }
    });
}

-(void)logout:(id)sender
{
    [self destroySession];
    [self.navigationController popViewControllerAnimated:YES];
}

-(void)refreshSession:(NSTimer*)timer
{
    if(self.communicator != nil)
    {
        [session begin_refresh:nil exception:^(ICEException* ex)
            {
                [self.navigationController popToRootViewControllerAnimated:YES];

                // The session is invalid, clear.
                self.session = nil;

                // Clean up the remainder.
                [self destroySession];

                NSString* s = [NSString stringWithFormat:@"Lost connection with session!\n%@", ex];

                // open an alert with just an OK button
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                                message:s
                                                               delegate:nil
                                                      cancelButtonTitle:@"OK"
                                                      otherButtonTitles:nil];
                [alert show];
            }];
    }
}

-(void)removeCurrentBook
{
    DemoBookDescription *book = (DemoBookDescription *)[books objectAtIndex:currentIndexPath.row];
    
    [[book proxy] begin_destroy:nil exception:^(ICEException* ex) {
		[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
		
		// Ignore ICEObjectNotExistException
		if([ex isKindOfClass:[ICEObjectNotExistException class]])
		{
			return;
		}
		
		[self exception:ex];
	}];
    
    // Remove the book, and the row from the table.
    [books removeObjectAtIndex:currentIndexPath.row];        
    --nrows;
    [searchTableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:currentIndexPath] 
                           withRowAnimation:UITableViewRowAnimationFade];
}

-(void)addBook:(id)sender
{
    DemoBookDescription* book = [DemoBookDescription bookDescription];
    book.title = @"";
    book.authors = [NSMutableArray array];
    
    [addController startEdit:book library:library];

    [self.navigationController pushViewController:addController animated:YES];
}

#pragma mark DetailControllerDelegate

-(void)bookUpdated:(DemoBookDescription*)book
{
    [books replaceObjectAtIndex:currentIndexPath.row withObject:book];
    [searchTableView reloadData];
}

-(void)bookDeleted
{
    [self removeCurrentBook];
}

#pragma mark AMI callbacks

-(void)exception:(ICEException*)ex
{
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;

    // BUGFIX: In the event of a fatal exception we want to pop back to the login view.
    // However, doing so directly by calling [self.navigationController popToRootViewControllerAnimated:YES];
    // causes the navigation view & the bar to get out of sync. So instead, we pop to the root view
    // in the alert view didDismissWithButtonIndex callback.
    [self destroySession];
    
    // open an alert with just an OK button
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                     message:[ex description]
                                                    delegate:self
                                           cancelButtonTitle:@"OK"
                                           otherButtonTitles:nil];
    [alert show];
}

#pragma mark UIAlertViewDelegate

-(void)alertView:(UIAlertView*)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if(session == nil)
    {
        [self.navigationController popToRootViewControllerAnimated:YES];
    }
}

#pragma mark UISearchBarDelegate

-(void)searchBarCancelButtonClicked:(UISearchBar*)sender
{
    [sender resignFirstResponder];
    searchSegmentedControl.hidden = YES;
    sender.showsCancelButton = NO;
}

-(void)searchBarSearchButtonClicked:(UISearchBar*)sender
{
    [sender resignFirstResponder];
    searchSegmentedControl.hidden = YES;
    sender.showsCancelButton = NO;
    
    // Initiate a search.
    NSString* search = sender.text;

    NSUInteger searchMode = searchSegmentedControl.selectedSegmentIndex;

    // Kill the previous query results.
    self.query = nil;
    nrows = 0;
    rowsQueried = 10;
    [books removeAllObjects];
    [searchTableView reloadData];
    
    // Run the query.
	void(^queryResponse)(NSMutableArray*, int, id<DemoBookQueryResultPrx>) = 
    ^(NSMutableArray* seq, int n, id<DemoBookQueryResultPrx> q) 
    {
        [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
		nrows = n;
		if(nrows == 0)
		{
			// open an alert with just an OK button
			UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Results"
															 message:@"The search returned no results"
															delegate:self
												   cancelButtonTitle:@"OK"
												   otherButtonTitles:nil];
			[alert show];
			return;
		}
		
		[books addObjectsFromArray:seq];
		self.query = q;
		
		[searchTableView reloadData];
    };
	
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
    if(searchMode == 0) // ISBN
    {
        [library begin_queryByIsbn:search
								 n:10
                          response:queryResponse
                         exception:^(ICEException* ex) { [self exception:ex]; }];
    }
    else if(searchMode == 1) // Authors
    {
        [library begin_queryByAuthor:search
								   n:10
                            response:queryResponse
                           exception:^(ICEException* ex) { [self exception:ex]; }];
    }
    else // Title
    {
        
        [library begin_queryByTitle:search
								  n:10
                           response:queryResponse
                          exception:^(ICEException* ex) { [self exception:ex]; }];
    }
}

-(BOOL)searchBarShouldBeginEditing:(UISearchBar *)sender
{
    searchSegmentedControl.hidden = NO;
    sender.showsCancelButton = YES;
    
    return YES;
}

#pragma mark UITableViewDelegate and UITableViewDataSource methods

-(void)tableView:(UITableView *)tv commitEditingStyle:(UITableViewCellEditingStyle)editingStyle
forRowAtIndexPath:(NSIndexPath *)indexPath
{
    // If row is deleted, remove it from the list.
    if(editingStyle == UITableViewCellEditingStyleDelete)
    {
        self.currentIndexPath = indexPath;
        [self removeCurrentBook];
    }
}

-(UITableViewCellEditingStyle)tableView:(UITableView *)tv editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return UITableViewCellEditingStyleDelete;
}

// These methods are all part of either the UITableViewDelegate or UITableViewDataSource protocols.

// This table will always only have one section.
-(NSInteger)numberOfSectionsInTableView:(UITableView *)tv
{
    return 1;
}

// One row per book, the number of books is the number of rows.
-(NSInteger)tableView:(UITableView *)tv numberOfRowsInSection:(NSInteger)section
{
    return nrows;
}

-(UITableViewCell *)tableView:(UITableView *)tv cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [searchTableView dequeueReusableCellWithIdentifier:@"MyIdentifier"];
    if (cell == nil)
    {
        // Create a new cell. CGRectZero allows the cell to determine the appropriate size.
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"MyIdentifier"];
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    }

    if(indexPath.row > books.count-1)
    {
        // Here we are past the available cached set of data. rowsQueried records
        // how many rows of data we've actually asked for.
        if(indexPath.row > rowsQueried-1)
        {
            [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
            NSAssert(query != nil, @"query != nil");
            [query begin_next:10
                     response:^(NSMutableArray* seq, BOOL destroyed) { 
						 [books addObjectsFromArray:seq];
						 // The query has returned all available results.
						 if(destroyed)
						 {
							 self.query = nil;
						 }
						 
						 [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
						 [searchTableView reloadData];
					 }
                    exception:^(ICEException* ex) { [self exception:ex]; }];
            rowsQueried += 10;
        }

        [cell.textLabel setText:@"<loading>"];
    }
    else
    {
        DemoBookDescription *book = (DemoBookDescription *)[books objectAtIndex:indexPath.row];
        [cell.textLabel setText:book.title];
    }
    return cell;
}

-(NSIndexPath *)tableView:(UITableView *)tv willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Selecting a <loading> book does nothing.
    if(indexPath.row > books.count-1)
    {
        return nil;
    }
    
    self.currentIndexPath = indexPath;

    [detailController startEdit:[books objectAtIndex:indexPath.row]];
    
    // Push the detail view on to the navigation controller's stack.
    [self.navigationController pushViewController:detailController animated:YES];
    return nil;
}

@end
