// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <LibraryController.h>
#import <AppDelegate.h>
#import <Library.h>
#import <Session.h>
#import <EditController.h>
#import <SavingController.h>
#import <RentController.h>

#import <objc/Glacier2.h>
#import <objc/Ice.h>

@implementation LibraryController

-(id)initWithCommunicator:(id<ICECommunicator>)c
                  session:(id)s
                   router:(id<GLACIER2RouterPrx>)r
           sessionTimeout:(int)t
                  library:(id<DemoLibraryPrx>)l
{
    if(self = [super initWithWindowNibName:@"LibraryView"])
    {
        communicator = c;
        session = s;
        router = r;
        library = l;
        
        books = [NSMutableArray array];
        rowsQueried = 0;
        nrows = 0;
        sessionTimeout = t;
        searchType = 0;
    }
    return self;
}

-(void)awakeFromNib
{
    NSApplication* app = [NSApplication sharedApplication];
    AppDelegate* delegate = (AppDelegate*)[app delegate];
    [delegate setLibraryActive:YES];
    
    // Setup the session refresh timer.
    refreshTimer = [NSTimer timerWithTimeInterval:sessionTimeout/2
                                           target:self
                                         selector:@selector(refreshSession:)
                                         userInfo:nil
                                          repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:refreshTimer forMode:NSDefaultRunLoopMode];
    [[titleField textStorage] setAttributedString:[[NSAttributedString alloc] initWithString:@""]];
}

#pragma mark Toolbar delegate

- (BOOL)validateToolbarItem:(NSToolbarItem *)toolbarItem
{
    // If there is no session everything is deactivated.
    if(!session)
    {
        return NO;
    }
    // Otherwise check the actions.
    if(toolbarItem.action == @selector(add:) || toolbarItem.action == @selector(search:))
    {
        return YES;
    }
    
    if(!selection)
    {
        return NO;
    }
    
    if(toolbarItem.action == @selector(rentBook:))
    {
        return selection.rentedBy.length == 0;
    }
    else if(toolbarItem.action == @selector(returnBook:))
    {
        return selection.rentedBy.length != 0;
    }
    return YES;
}

#pragma mark Session management

-(void)destroySession
{
    if(refreshTimer)
    {
        [refreshTimer invalidate];
        refreshTimer = nil;
    }
    
    // Destroy the session.
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
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
		
		// Destroy might block so we call it from a separate thread.
		[communicator destroy];
		communicator = nil;
		
		dispatch_async(dispatch_get_main_queue(), ^ {
			NSApplication* app = [NSApplication sharedApplication];
			AppDelegate* delegate = (AppDelegate*)[app delegate];
			[delegate setLibraryActive:NO];
		});
    });
}

-(void)windowWillClose:(NSNotification *)notification
{
    [self destroySession];
}

-(void)setSelection:(DemoBookDescription*)description
{
    selection = description;
    if(description)
    {
        isbnField.stringValue = description.isbn;
        [[titleField textStorage] setAttributedString:[[NSAttributedString alloc] initWithString:description.title]];
        renterField.stringValue = description.rentedBy;
    }
    else
    {
        isbnField.stringValue = @"";
        [[titleField textStorage] setAttributedString:[[NSAttributedString alloc] initWithString:@""]];
        renterField.stringValue = @"";
    }
    [authorsTable reloadData];
}

-(void)exception:(ICEException*)ex
{
    [searchIndicator stopAnimation:self];
    if(savingController)
    {
        // Hide the saving sheet.
        [NSApp endSheet:savingController.window];
        [savingController.window orderOut:self]; 
		
        savingController = nil;
    }
	
    NSString* s;
    
    if([ex isKindOfClass:[ICEObjectNotExistException class]])
    {
        if(selection)
        {
            [books removeObjectAtIndex:queryTable.selectedRow];
            --nrows;
            [queryTable reloadData];
        }
    }
    else if([ex isKindOfClass:[DemoBookRentedException class]])
    {
        DemoBookRentedException* ex2 = (DemoBookRentedException*)ex;
        s = @"The book has already been rented";
        
        NSAssert(selection != nil, @"selection != nil");
        selection.rentedBy = ex2.renter;
        [self setSelection:selection]; // Update detail
        [queryTable reloadData];
    }
    else if([ex isKindOfClass:[DemoBookNotRentedException class]])
    {
        s = @"The book has already been returned.";
        NSAssert(selection != nil, @"selection != nil");        
        selection.rentedBy = @"";
        [self setSelection:selection]; // Update detail
        [queryTable reloadData];
    }
    else
    {
        // Unknown exception.
        s = [ex description];
        [self destroySession];
    }
    
    NSRunAlertPanel(@"Error", @"%@", @"OK", nil, nil, s);
    
    if(editController && session)
    {
        [NSApp beginSheet:editController.window 
           modalForWindow:self.window
            modalDelegate:self 
           didEndSelector:@selector(editFinished:returnCode:contextInfo:) 
              contextInfo:NULL];
    }
}

-(void)refreshSession:(NSTimer*)timer
{
    if(session != nil)
    {
        [session begin_refresh:nil exception:^(ICEException *ex)
            {
                [self destroySession];
                if(savingController)
                {
                    // Hide the saving sheet.
                    [NSApp endSheet:savingController.window];
                    [savingController.window orderOut:self]; 
                    savingController = nil;
                }
                NSRunAlertPanel(@"Error", @"%@", @"OK", nil, nil, [ex description]);
            }];
    }
}

-(void)logout:(id)sender
{
    [self destroySession];
}

-(void)asyncRequestReply
{
    // If the current selection was updated, then modify the cached search results.
    if(updated && selection)
    {
        [books replaceObjectAtIndex:[queryTable selectedRow] withObject:updated];
        [self setSelection:updated];
    }
    updated = nil;
    
    // Close the saving controller.
    NSAssert(savingController != nil, @"savingController != nil");
    [NSApp endSheet:savingController.window];
    [savingController.window orderOut:self];
	
    // Clear the state variables.
    savingController = nil;
    editController = nil;
    rentController = nil;
}

#pragma mark New Book

- (void)addFinished:(NSWindow *)sheet returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{
    [sheet orderOut:self];
	
    if(returnCode)
    {
        savingController = [[SavingController alloc] init];
        [NSApp beginSheet:savingController.window
           modalForWindow:self.window
            modalDelegate:nil 
           didEndSelector:NULL 
              contextInfo:NULL];
        
        DemoBookDescription* result = editController.result;
        [library begin_createBook:result.isbn
							title:result.title
                          authors:result.authors
                         response:^(id<DemoBookPrx> b) { [self asyncRequestReply]; }
                        exception:^(ICEException* ex) { [self exception:ex]; }];
    }
    else
    {
        editController = nil;
    }
}

-(void)add:(id)sender
{
	editController = [[EditController alloc] initWithDesc:nil];
    [NSApp beginSheet:editController.window
       modalForWindow:self.window
        modalDelegate:self
       didEndSelector:@selector(addFinished:returnCode:contextInfo:) 
          contextInfo:NULL];
}

#pragma mark Remove Book

-(void)remove:(id)sender
{
    if(NSRunAlertPanel(@"Confirm", @"Remove book?", @"OK", @"Cancel", nil) == NSAlertDefaultReturn)
    {
        [selection.proxy begin_destroy:nil exception:^(ICEException* ex) {
			if([ex isKindOfClass:[ICEObjectNotExistException class]]) // Ignore ICEObjectNotExistException
			{
				[searchIndicator stopAnimation:self];
				return;
			}	    
			[self exception:ex];
		}];
        
        // Remove the book, and the row from the table.
        [books removeObjectAtIndex:queryTable.selectedRow];        
        --nrows;
        [queryTable reloadData];
    }
}

#pragma mark Query

-(void)setSearchType:(id)sender
{
    NSMenuItem* item = sender;
    searchType = item.tag;
}

- (BOOL)validateMenuItem:(NSMenuItem *)item
{
    if([item action] == @selector(setSearchType:))
    {
        [item setState:(item.tag == searchType) ? NSOnState : NSOffState];        
    }
    if ([item action] == @selector(logout:))
    {
        return session != nil;
    }
	return YES;
}

-(void)search:(id)sender
{
    // Kill the previous query results.
    query = nil;
    nrows = 0;
    rowsQueried = 10;
    [books removeAllObjects];
    // No need to call setSelection: since the selection will change to none automatically.
    [queryTable reloadData];
    
    NSString* s = searchField.stringValue;
    s = [s stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    if(s.length == 0)
    {
        return;
    }
    [searchIndicator startAnimation:self];
    
    // Run the query.
    void(^queryResponse)(NSMutableArray*, int, id<DemoBookQueryResultPrx>) = 
    ^(NSMutableArray* seq, int n, id<DemoBookQueryResultPrx> q) 
    {
		[searchIndicator stopAnimation:self];
		nrows = n;
		if(nrows == 0)
		{
			return;
		}
		
		[books addObjectsFromArray:seq];
		query = q;
		[queryTable reloadData];
    };
    
    switch(searchType)
    {
        case 0: // ISBN
        {
            [library begin_queryByIsbn:s
                                     n:10
                              response:queryResponse
                             exception:^(ICEException* ex) { [self exception:ex]; }];
            break;
        }
        case 1: // Authors
        {
            [library begin_queryByAuthor:s
									   n:10
                                response:queryResponse
							   exception:^(ICEException* ex) { [self exception:ex]; }];
            break;
        }
        case 2: // Title
        {
            [library begin_queryByTitle:s
									  n:10
                               response:queryResponse
							  exception:^(ICEException* ex) { [self exception:ex]; }];
            break;
        }
        default:
        {
            break;
        }
    }
}

#pragma mark Edit Book

- (void)editFinished:(NSWindow *)sheet returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{
    [sheet orderOut:self];
    
    if(returnCode)
    {
        savingController = [[SavingController alloc] init];
        [NSApp beginSheet:savingController.window
           modalForWindow:self.window
            modalDelegate:nil 
           didEndSelector:NULL 
              contextInfo:NULL];
		
        updated = editController.result;
		
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
			@try
			{
				if(![updated.title isEqualToString:selection.title])
				{
					[updated.proxy setTitle:updated.title];
				}
				BOOL diff = NO;
				if(updated.authors.count == selection.authors.count)
				{
					for(int i = 0; i < updated.authors.count; ++i)
					{
						if(![[updated.authors objectAtIndex:i] isEqualToString:[selection.authors objectAtIndex:i]])
						{
							diff = YES;
							break;
						}
					}
				}
				else
				{
					diff = YES;
				}
				if(diff)
				{
					[updated.proxy setAuthors:updated.authors];
				}
				dispatch_async(dispatch_get_main_queue(), ^{ [self asyncRequestReply]; });
			}
			@catch(ICEException* ex)
			{
				dispatch_async(dispatch_get_main_queue(), ^{ [self exception:ex]; });
			}
		});
    }
}

-(void)edit:(id)sender
{
    editController = [[EditController alloc] initWithDesc:selection];
	
    [NSApp beginSheet:editController.window 
       modalForWindow:self.window
        modalDelegate:self 
       didEndSelector:@selector(editFinished:returnCode:contextInfo:) 
          contextInfo:NULL];
}

#pragma mark Rent Book

- (void)rentFinished:(NSWindow *)sheet returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{
    [sheet orderOut:self];
    if(returnCode)
    {
        savingController = [[SavingController alloc] init];
        [NSApp beginSheet:savingController.window
           modalForWindow:self.window
            modalDelegate:nil 
           didEndSelector:NULL 
              contextInfo:NULL];
		
        updated = [selection copy];
        updated.rentedBy = rentController.renter;
		
        [selection.proxy begin_rentBook:rentController.renter
                               response:^ { [self asyncRequestReply]; }
                              exception:^(ICEException* ex) { [self exception:ex]; }];
    }
    else
    {
        rentController = nil;
    }
}

-(void)rentBook:(id)sender
{
    if(selection)
    {
        rentController = [[RentController alloc] init];
        [NSApp beginSheet:rentController.window 
           modalForWindow:self.window
            modalDelegate:self 
           didEndSelector:@selector(rentFinished:returnCode:contextInfo:) 
              contextInfo:NULL];
    }
}

-(void)returnBook:(id)sender
{
    if(selection)
    {
        savingController = [[SavingController alloc] init];
        [NSApp beginSheet:savingController.window
           modalForWindow:self.window
            modalDelegate:nil 
           didEndSelector:NULL 
              contextInfo:NULL];
        
        updated = [selection copy];
        updated.rentedBy = @"";
        
        [selection.proxy begin_returnBook:^ { [self asyncRequestReply]; }
								exception:^(ICEException* ex) { [self exception:ex]; }];
    }
}

#pragma mark NSTableView delegate

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
    if([aNotification object] == queryTable)
    {
        int index = queryTable.selectedRow;
        if(index == -1)
        {
            [self setSelection:nil];
        }
        else
        {
            [self setSelection:[books objectAtIndex:index]];
        }
    }
}

#pragma mark NSTableViewDataSource methods

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    if(aTableView == queryTable)
    {
        return nrows;
    }
    else
    {
        return selection.authors.count;
    }
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    if(aTableView == queryTable)
    {
        if(rowIndex > books.count-1)
        {
            // Here we are past the available cached set of data. rowsQueried records
            // how many rows of data we've actually asked for.
            if(rowIndex > rowsQueried-1)
            {
                [searchIndicator startAnimation:self];
                NSAssert(query != nil, @"query != nil");
                [query begin_next:10
						 response:^(NSMutableArray* seq, BOOL destroyed) { 
							 [searchIndicator stopAnimation:self];
							 [books addObjectsFromArray:seq];
							 // The query has returned all available results.
							 if(destroyed)
							 {
								 query = nil;
							 }
							 [queryTable reloadData];
						 }
                        exception:^(ICEException* ex) { [self exception:ex]; } ];
                rowsQueried += 10;
            }
            
            return @"<loading>";
        }
        else
        {
            DemoBookDescription *book = (DemoBookDescription *)[books objectAtIndex:rowIndex];
            NSString* id = aTableColumn.identifier;
            if([id isEqualToString:@"ISBN"])
            {
                return book.isbn;
            }
            else // Title
            {
                return book.title;
            }
            
        }
    }
    else
    {
        return [selection.authors objectAtIndex:rowIndex];
    }
}

@end
