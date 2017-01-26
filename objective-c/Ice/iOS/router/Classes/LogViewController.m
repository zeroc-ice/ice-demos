// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************


#import <LogViewController.h>
#import <AppDelegate.h>
#import <LogZoomViewController.h>
#import <RootViewController.h>

@interface MessageCell : UITableViewCell
{
@private
    UILabel* timestamp;
    UILabel* body;
    UILabel* type;
    UILabel* category;
    LogEntry* entry;
    NSDateFormatter *dateFormatter;
}

@property (nonatomic) UILabel* timestamp;
@property (nonatomic) UILabel* body;
@property (nonatomic) UILabel* type;
@property (nonatomic) UILabel* category;
@property (nonatomic) LogEntry* entry;
@property (nonatomic) NSDateFormatter* dateFormatter;

+(CGFloat)heightForMessage:(LogEntry*)entry;

@end

@implementation MessageCell
@synthesize timestamp;
@synthesize type;
@synthesize category;
@synthesize body;
@synthesize entry;
@synthesize dateFormatter;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])
    {
        self.category = [[UILabel alloc] initWithFrame:CGRectZero];
        
        self.category.textAlignment = NSTextAlignmentLeft;
        self.category.textColor = [UIColor blueColor];
        self.category.font = [UIFont boldSystemFontOfSize:12];
        self.category.numberOfLines = 0;
        
        self.type = [[UILabel alloc] initWithFrame:CGRectZero];
        
        self.type.textAlignment = NSTextAlignmentLeft;
        self.type.textColor = [UIColor blueColor];
        self.type.font = [UIFont boldSystemFontOfSize:12];
        self.type.numberOfLines = 0;
        
        self.timestamp = [[UILabel alloc] initWithFrame:CGRectZero];
        self.timestamp.textAlignment = NSTextAlignmentRight;
        self.timestamp.textColor = [UIColor blackColor];
        self.timestamp.highlightedTextColor = [UIColor darkGrayColor];
        self.timestamp.font = [UIFont boldSystemFontOfSize:12];
        self.timestamp.numberOfLines = 0;
        
        self.body = [[UILabel alloc] initWithFrame:CGRectZero];
        
        self.body.textColor = [UIColor lightGrayColor];
        self.body.font = [UIFont boldSystemFontOfSize:14];
        self.body.numberOfLines = 0;
        
        [self.contentView addSubview:self.timestamp];
        [self.contentView addSubview:self.type];
        [self.contentView addSubview:self.category];
        [self.contentView addSubview:self.body];
        
        self.dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateStyle:NSDateFormatterShortStyle];
        [dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
    }
    
    return self;
}

+(CGFloat)heightForMessage:(LogEntry*)entry
{
    // The header is always one line, the body is multiple lines.
    // The width of the table is 320 - 20px of left & right padding. We don't want to let the body
    // text go past 200px.
    
    CGRect rect = [entry.message boundingRectWithSize:CGSizeMake(300.f, 200.0f) options:NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading attributes:@{NSFontAttributeName:[UIFont boldSystemFontOfSize:14] } context:nil];
    return rect.size.height + 20.f + 20.f; // 20px padding.
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    
    CGRect contentRect = self.contentView.bounds;
    
    CGRect timestampFrame = CGRectMake(160.f, 0.0f, 150.f, 20.f);
    self.timestamp.frame = timestampFrame;

    CGRect bodyFrame = CGRectMake(10.f, 20.f, CGRectGetWidth(contentRect)-20.f, CGRectGetHeight(contentRect)-20.f);
    self.body.frame = bodyFrame;
    
    CGRect typeFrame = CGRectMake(10.f, 0.0f, 60.f, 20.f);
    self.type.frame = typeFrame;
    
    CGRect categoryFrame = CGRectMake(70.f, 0.0f, 60.f, 20.f);
    self.category.frame = categoryFrame;
}


-(void)setEntry:(LogEntry*)m
{
    // Don't call self.message here, that results in infinite recursion.
    entry = m;
    self.category.text = entry.category;
    self.timestamp.text = [dateFormatter stringFromDate:entry.timestamp];
    switch(entry.type)
    {
        case LogEntryTypePrint:
            self.type.text = @"Print";
            break;
        case LogEntryTypeTrace:
            self.type.text = @"Trace";
            break;
        case LogEntryTypeWarning:
            self.type.text = @"Warning";
            break;
        case LogEntryTypeError:
            self.type.text = @"Error";
            break;
        default:
            self.type.text = @"UNKNOWN";
    }
    self.body.text = entry.message;
}

@end

@implementation LogViewController

@synthesize messages;
@synthesize messagesTableView;
@synthesize logZoomViewController;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        self.messages = [NSMutableArray array];
        
        AppDelegate* app = (AppDelegate*)[[UIApplication sharedApplication] delegate];
        app.logger.delegate = self;
    }
    return self;
}


- (void)viewDidLoad {
    [super viewDidLoad];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}



#pragma mark LoggingDelegate

-(void)log:(LogEntry*)s
{
    if(messages.count > 100) // max 100 messages
    {
        [messages removeObjectAtIndex:0];
    }
    [messages addObject:s];
    [messagesTableView reloadData];
    NSUInteger path[] = {0, messages.count-1};
    [messagesTableView scrollToRowAtIndexPath:[NSIndexPath indexPathWithIndexes:path length:2] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
}

#pragma mark UITableView

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return messages.count;
}

-(UITableViewCell *)tableView:(UITableView *)tv cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    MessageCell *cell = (MessageCell*)[messagesTableView dequeueReusableCellWithIdentifier:@"MessageCell"];
    if(cell == nil)
    {
        // Create a new cell. CGRectZero allows the cell to determine the appropriate size.
        cell = [[MessageCell alloc] initWithFrame:CGRectZero];
    }
    cell.entry = [messages objectAtIndex:indexPath.row];
    return cell;
}

-(NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    AppDelegate* app = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    
    LogZoomViewController* controller = app.rootViewController.logZoomViewController;
    controller.messages = self.messages;
    controller.current = (int)indexPath.row;
    [app.rootViewController toggleLogZoomView];

    return nil;
}


@end
