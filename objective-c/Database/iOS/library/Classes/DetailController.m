// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <DetailController.h>
#import <EditController.h>
#import <AppDelegate.h>

#import <Library.h>

#import <objc/Ice.h>

static EditController* editViewController_ = nil;

@interface DetailController ()

@property (nonatomic) UITableView *tableView;
@property (nonatomic) NSIndexPath *selectedIndexPath;
@property (nonatomic, copy) DemoBookDescription* updated;

@end

@implementation DetailController

@synthesize selectedIndexPath;
@synthesize tableView;
@synthesize book;
@synthesize updated;
@synthesize waitAlert;
@synthesize delegate;

-(id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
	if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])
    {
        self.title = @"Info";
	}
	return self;
}

-(void)popBack
{
    if(changed)
    {
        [delegate bookUpdated:book];
    }
    [self.navigationController popViewControllerAnimated:YES];
}

-(void)viewDidLoad
{
    self.navigationItem.rightBarButtonItem = self.editButtonItem;
    tableView.allowsSelectionDuringEditing = YES;
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Search"
                                                                             style:UIBarButtonItemStylePlain
                                                                            target:self
                                                                            action:@selector(popBack)];
}

-(void)viewWillAppear:(BOOL)animated
{
    self.navigationItem.leftBarButtonItem.action = @selector(popBack);
    self.navigationItem.leftBarButtonItem.target = self;
    
    // Remove any existing selection.
    [tableView deselectRowAtIndexPath:selectedIndexPath animated:NO];
    // Redisplay the data.
    [tableView reloadData];
}

-(void)setEditing:(BOOL)editing animated:(BOOL)animated
{
    [super setEditing:editing animated:animated];
    [tableView setEditing:editing animated:animated];
    [self.navigationItem setHidesBackButton:editing animated:animated];
    [tableView reloadData];
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


+ (EditController *)editViewController
{
    // Instantiate the edit view controller if necessary.
    if(editViewController_ == nil)
    {
        editViewController_ = [[EditController alloc] initWithNibName:@"EditView" bundle:nil];
    }
    return editViewController_;
}

-(IBAction)removeBook:(id)sender
{
    [self.navigationController popViewControllerAnimated:YES];
    [delegate bookDeleted];
}

-(void)startEdit:(DemoBookDescription*)b
{
    fatal = NO;
    changed = NO;
    self.book = b;
    [self setEditing:NO animated:NO];
}

#pragma mark AMI Callbacks

-(void)exception:(ICEException*)ex
{
    [self saving:NO];
    // Discard the edit.
    self.updated = nil;


    [self.tableView deselectRowAtIndexPath:selectedIndexPath animated:NO];
    
    NSString* s;
    
    // Ignore ObjectNotExistExceptiojn
    if([ex isKindOfClass:[ICEObjectNotExistException class]])
    {
        [self.navigationController popViewControllerAnimated:YES];
        [delegate bookDeleted];
        s = @"The current book has been removed";
    }
    else if([ex isKindOfClass:[DemoBookRentedException class]])
    {
        DemoBookRentedException* ex2 = (DemoBookRentedException*)ex;
        s = @"The book has already been rented";

        book.rentedBy = ex2.renter;
        [tableView reloadData];
    }
    else if([ex isKindOfClass:[DemoBookNotRentedException class]])
    {
        s = @"The book has already been returned.";

        book.rentedBy = @"";
        [tableView reloadData];
    }
    else
    {
        s = [ex description];

        // BUGFIX: In the event of a fatal exception we want to pop back to the login view.
        // However, doing so directly by calling [self.navigationController popToRootViewControllerAnimated:YES];
        // causes the navigation view & the bar to get out of sync. So instead, we pop to the root view
        // in the alert view didDismissWithButtonIndex callback.
        fatal = YES;
    }

    // open an alert with just an OK button
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                     message:s
                                                    delegate:self
                                           cancelButtonTitle:@"OK"
                                           otherButtonTitles:nil];
    [alert show];
}

#pragma mark Callbacks from EditView

-(void)saving:(BOOL)status
{
    saving = status;
    [tableView reloadData];
    if(saving)
    {
        [statusActivity startAnimating];
        tableView.alpha = 0.5;
    }
    else
    {
        [statusActivity stopAnimating];
        tableView.alpha = 1.0;
    }
    statusLabel.hidden = !saving;
}

-(void)commitEdit
{
    self.book = updated;
    self.updated = nil;
    
    changed = YES;
    
    [self saving:NO];
}

-(void)saveIsbn:(NSString*)value
{
    NSAssert(book.proxy == nil, @"book.proxy == nil");
    self.updated = book;
    updated.isbn = value;

    [self commitEdit];
}

-(void)saveTitle:(NSString*)title
{
    [self saving:YES];
    self.updated = book;
    updated.title = title;
    
    if(book.proxy != nil)
    {
        [[book proxy] begin_setTitle:title
                            response:^ { [self commitEdit]; }
						   exception:^(ICEException* ex) { [self exception:ex]; }];
	}
    else
    {
        [self commitEdit];
    }
}

-(void)saveAuthors:(NSString*)value
{
    [self saving:YES];
    NSUInteger index = selectedIndexPath.row;

    self.updated = book;
    
    NSMutableArray* arr = (NSMutableArray*)updated.authors;
    if(index == book.authors.count)
    {
        [arr addObject:value];
    }
    else
    {
        [arr replaceObjectAtIndex:index withObject:value];
    }
    
    if(book.proxy != nil)
    {
        [[book proxy] begin_setAuthors:arr
							  response:^ { [self commitEdit]; }
							 exception:^(ICEException* ex) { [self exception:ex]; }];
    }
    else
    {
        [self commitEdit];
    }    
}

-(void)rentBook:(NSString*)value
{
    [self saving:YES];
    self.updated = book;
    updated.rentedBy = value;
    
    NSAssert(book.proxy != nil, @"book.proxy != nil");
    
    [[book proxy] begin_rentBook:value
						response:^ { [self commitEdit]; }
					   exception:^(ICEException* ex) { [self exception:ex]; }];
}

#pragma mark UIAlertViewDelegate

-(void)alertView:(UIAlertView*)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if(fatal)
    {
        [delegate destroySession];
        [self.navigationController popToRootViewControllerAnimated:YES];
    }
}

#pragma mark <UITableViewDelegate, UITableViewDataSource> Methods

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tv
{
    // If we're in "add book mode" then the rented-by section is not visible.
    if(book.proxy == nil)
    {
        return 3;
    }

    // 4 sections. ISBN, Title, Authors, Rented By | Remove books.
    return 4;
}

-(NSInteger)tableView:(UITableView *)tv numberOfRowsInSection:(NSInteger)section
{
    if(section == 2) // Authors
    {
        if(self.editing)
        {
            return book.authors.count+1;
        }
        return book.authors.count;
    }
    return 1;
}

-(UITableViewCellEditingStyle)tableView:(UITableView *)tv editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(!self.editing || saving|| indexPath.section != 2)
    {
        return UITableViewCellEditingStyleNone;
    }

    if(indexPath.row == book.authors.count)
    {
        return UITableViewCellEditingStyleInsert;
    }
    else
    {
        return UITableViewCellEditingStyleDelete;
    }
}

-(void)tableView:(UITableView *)tv commitEditingStyle:(UITableViewCellEditingStyle)editingStyle
                                    forRowAtIndexPath:(NSIndexPath *)indexPath
{
    // If row is deleted, remove it from the list.
    if(editingStyle == UITableViewCellEditingStyleDelete)
    {
        self.updated = book;
        [self saving:YES];
        // Remove the author from the book.
        NSMutableArray* arr = (NSMutableArray*)updated.authors;
        [arr removeObjectAtIndex:indexPath.row];
        
        if(book.proxy != nil)
        {
            [[book proxy] begin_setAuthors:arr
								  response:^ { [self commitEdit]; }
								 exception:^(ICEException* ex) { [self exception:ex]; }];
        }
        else
        {
            [self commitEdit];
        }    
        
        // Animate the deletion from the table.
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] 
                         withRowAnimation:UITableViewRowAnimationFade];
    }
    else if(editingStyle == UITableViewCellEditingStyleInsert)
    {
        EditController *controller = [DetailController editViewController];
        self.selectedIndexPath = indexPath;
        NSString* auth = (indexPath.row >= book.authors.count) ? @"" : [book.authors objectAtIndex:indexPath.row];
        [controller startEdit:self selector:@selector(saveAuthors:) name:@"Author" value:auth];
        
        [self.navigationController pushViewController:controller animated:YES];
    }
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    if(self.editing && section == 3) // Remove button.
    {
        return 0.f;
    }
    return self.tableView.sectionHeaderHeight;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(indexPath.section == 1)
    {
        if(book.title.length == 0)
        {
            return 44.f;
        }
        
        // The width of the table is 320 - 20px of left & right padding. We don't want to let the title
        // go past 200px.
        CGSize rectSz = self.editing ? CGSizeMake(250.f, 200.0f) : CGSizeMake(260.f, 200.0f);
        CGRect rect = [book.title boundingRectWithSize:rectSz options:NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading attributes:@{NSFontAttributeName:[UIFont boldSystemFontOfSize:20] } context:nil];
        return rect.size.height + 20.f; // 20px padding.
    }
    else
    {
        return 44.0f; // 20px padding + 22.f for the 20pt font.
    }
}

-(UITableViewCell *)tableView:(UITableView *)tv cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell;
    // 4 Possible sections. Section 1 and 4 are special. The title section, and the remove book section.
    switch(indexPath.section)
    {
        case 1: // Title
        {
            cell = [tableView dequeueReusableCellWithIdentifier:@"TitleCell"];
            if (cell == nil)
            {
                // Create a new cell.
                cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"TitleCell"];
                // Add a label to the frame,
                UILabel *textView = [[UILabel alloc] initWithFrame:CGRectZero];
                [cell.contentView addSubview:textView];
                textView.numberOfLines = 0;
                textView.textColor = [UIColor blackColor];
                textView.font = [UIFont boldSystemFontOfSize:20];
            }
            
            UILabel* textView = [cell.contentView.subviews objectAtIndex:0];
            
            CGSize rectSz = self.editing ? CGSizeMake(250.f, 200.0f) : CGSizeMake(260.f, 200.0f);
            CGRect rect = [book.title boundingRectWithSize:rectSz options:NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading attributes:@{NSFontAttributeName:[UIFont boldSystemFontOfSize:20] } context:nil];
            textView.frame = CGRectMake(10.f, 10.f, rect.size.width, rect.size.height);
            textView.text = book.title;
            break;
        }
            
        case 3: // Remove book section OR rented by section
        {
            if(self.editing)
            {
                // Remove book section.
                cell = [tableView dequeueReusableCellWithIdentifier:@"RemoveBook"];
                if (cell == nil)
                {
                    // Create a new cell.
                    cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                                   reuseIdentifier:@"RemoveBook"];
                    // Add a label to the frame,
                    
                    UIImage *buttonBackground = [UIImage imageNamed:@"redButton.png"];
                    UIImage *newImage = [buttonBackground stretchableImageWithLeftCapWidth:12.0 topCapHeight:0.0];
                    
                    UIButton* button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
                    button.tag = 100;
                    button.frame = CGRectMake(-30.f, 0.f, 300.f, 44.f);
                    
                    [button setTitle:@"Remove Book" forState:UIControlStateNormal];
                    [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
                    [button setBackgroundImage:newImage forState:UIControlStateNormal];
                    
                    [button addTarget:self action:@selector(removeBook:) forControlEvents:UIControlEventTouchUpInside];
                    
                    button.enabled = !saving;
                    [cell.contentView addSubview:button];
                }
                else
                {
                    UIButton* button = (UIButton*)[cell.contentView viewWithTag:100];
                    button.enabled = !saving;
                }
                break;
            }
            // fall through
        }
            
        default:
        {
            cell = [tableView dequeueReusableCellWithIdentifier:@"MyIdentifier"];
            if (cell == nil)
            {
                cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"MyIdentifier"];
            }
            
            if(indexPath.section == 0)
            {
                [cell.textLabel setText:book.isbn];
            }
            else if(indexPath.section == 2)
            {
                if(indexPath.row == book.authors.count)
                {
                    NSAssert(self.editing, @"self.editing");
                    [cell.textLabel setText:@"Add new author"];
                }
                else
                {
                    [cell.textLabel setText:[book.authors objectAtIndex:(indexPath.row)]];
                }
            }
            else if(indexPath.section == 3)
            {
                [cell.textLabel setText:book.rentedBy];
            }
        }
    }
    return cell;
}

-(NSString *)tableView:(UITableView *)tv titleForHeaderInSection:(NSInteger)section
{
    // Return the displayed title for the specified section.
    switch (section)
    {
        case 0:
            return @"ISBN";
        case 1:
            return @"Title";
        case 2:
            return @"Authors";
        case 3:
            if(!self.editing)
            {
                return @"Rented By";
            }
            break;
    }
    return nil;
}

-(NSIndexPath *)tableView:(UITableView *)tv willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(indexPath.section == 3)
    {
        return indexPath;
    }
    // Only allow selection if editing.
    return (self.editing) ? indexPath : nil;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    EditController *controller = [DetailController editViewController];
    //
    // Do not accept edit commands while saving is in progress.
    //
    if(saving)
    {
        return;
    }
    switch(indexPath.section)
    {
        case 0:
        {
            // Its not possible to change the ISBN number of an existing book.
            if(book.proxy != nil)
            {
                return;
            }
            
            [controller startEdit:self selector:@selector(saveIsbn:) name:@"ISBN" value:book.isbn];
            break;
        }
            
        case 1:
        {
            [controller startEdit:self selector:@selector(saveTitle:)name:@"Title" value:book.title];
            break;
        }
            
        case 2:
        {
            self.selectedIndexPath = indexPath;
            NSString* auth = (indexPath.row >= book.authors.count) ? @"" : [book.authors objectAtIndex:indexPath.row];
            [controller startEdit:self selector:@selector(saveAuthors:) name:@"Author" value:auth];
            break;
        }
            
        case 3:
        {
            // Its not possible to set the rented by field of a new book.
            NSAssert(book.proxy != nil, @"book.proxy != nil");

            if(book.rentedBy.length != 0)
            {
                // Return the book.
                UIActionSheet* sheet = [[UIActionSheet alloc] initWithTitle:@"Return Book"
                                                                    delegate:self
                                                           cancelButtonTitle:@"Cancel"
                                                      destructiveButtonTitle:@"Return" 
                                                           otherButtonTitles:nil];
                [sheet showInView:self.view];

                self.selectedIndexPath = indexPath;
                return;
            }

            [controller startEdit:self selector:@selector(rentBook:) name:@"" value:@""];
            break;
        }
    }

    self.selectedIndexPath = indexPath;

    [self.navigationController pushViewController:controller animated:YES];
}

#pragma mark - UIActionSheetDelegate

-(void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [self.tableView deselectRowAtIndexPath:selectedIndexPath animated:NO];
    if(buttonIndex == 0)
    {
        [self saving:YES];
        [[book proxy] begin_returnBook:^ {
            
            [self saving:NO];
			[self.tableView deselectRowAtIndexPath:selectedIndexPath animated:NO];
			
			book.rentedBy = @"";
			[tableView reloadData];
		}
        exception:^(ICEException* ex) { [self exception:ex]; }];
    }
}

@end
