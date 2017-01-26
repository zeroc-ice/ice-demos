// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <ChatController.h>
#import <UserController.h>

#import <objc/Ice.h>
#import <objc/Glacier2.h>

@interface ChatMessage : NSObject
{
@private
    NSDate* timestamp;
    NSString* who;
    NSString* text;
}
@property (nonatomic, readonly) NSString* text;
@property (nonatomic, readonly) NSString* who;
@property (nonatomic, readonly) NSDate* timestamp;

-(id)initWithText:(NSString*)text who:(NSString*)who timestamp:(ICELong)timestamp;
+(id)chatMessageWithText:(NSString*)text who:(NSString*)who timestamp:(ICELong)timestamp;
@end

@implementation ChatMessage

@synthesize who;
@synthesize text;
@synthesize timestamp;

-(id)initWithText:(NSString*)t who:(NSString*)w timestamp:(ICELong)ts
{
    if((self = [super init]))
    {
        who = w;

        // De-HTMLize the incoming message.
        NSMutableString* s = [t mutableCopy];
        NSString* replace[] =
        {
            @"&quot;",
            @"\"",
            @"&#39;", @"'",
            @"&lt;", @"<",
            @"&gt;", @">",
            @"&amp;", @"&"
        };
        int i;
        for(i = 0; i < sizeof(replace)/sizeof(replace[0]); i += 2)
        {
            [s replaceOccurrencesOfString:replace[i]
             withString:replace[i+1] options:NSCaseInsensitiveSearch
             range:NSMakeRange(0, s.length)];
        }
        
        text = s;
        
        // The ChatMessage timestamp is ms since the UNIX epoch.
        timestamp = [NSDate dateWithTimeIntervalSinceReferenceDate:(ts/ 1000.f) - NSTimeIntervalSince1970];
    }
    return self;
}

+(id)chatMessageWithText:(NSString*)text who:(NSString*)who timestamp:(ICELong)timestamp
{
    return [[ChatMessage alloc] initWithText:text who:who timestamp:timestamp];
}

@end

@interface MessageCell : UITableViewCell
{
@private
    UILabel* timestamp;
    UILabel* who;
    UILabel* body;
    NSDateFormatter *dateFormatter;

    ChatMessage* message;
}

@property (nonatomic) ChatMessage* message;

+(CGFloat)heightForMessage:(ChatMessage*)messsage;

@end

@implementation MessageCell
@synthesize message;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])
    {
        who = [[UILabel alloc] initWithFrame:CGRectZero];
        
        who.textAlignment = NSTextAlignmentLeft;
        who.textColor = [UIColor blueColor];
        who.font = [UIFont boldSystemFontOfSize:12];
        who.numberOfLines = 0;
        
        timestamp = [[UILabel alloc] initWithFrame:CGRectZero];
        timestamp.textAlignment = NSTextAlignmentRight;
        timestamp.textColor = [UIColor blackColor];
        timestamp.highlightedTextColor = [UIColor darkGrayColor];
        timestamp.font = [UIFont boldSystemFontOfSize:12];
        timestamp.numberOfLines = 0;
        
        body = [[UILabel alloc] initWithFrame:CGRectZero];

        body.textColor = [UIColor lightGrayColor];
        body.font = [UIFont boldSystemFontOfSize:14];
        body.numberOfLines = 0;
        
        [self.contentView addSubview:timestamp];
        [self.contentView addSubview:who];
        [self.contentView addSubview:body];
        
        dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateStyle:NSDateFormatterShortStyle];
        [dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
    }
    
    return self;
}

+(CGFloat)heightForMessage:(ChatMessage*)message
{
    // The header is always one line, the body is multiple lines.
    // The width of the table is 320 - 20px of left & right padding. We don't want to let the body
    // text go past 200px.
    CGRect body = [[message text] boundingRectWithSize:CGSizeMake(300.f, 200.0f) options:NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingUsesFontLeading attributes:@{NSFontAttributeName:[UIFont boldSystemFontOfSize:14] } context:nil];
    
    return body.size.height + 20.f; // 20px padding.
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    
    CGRect contentRect = self.contentView.bounds;
    
    CGRect timestampFrame = CGRectMake(160.f, 0.0f, 150.f, 20.f);
    CGRect whoFrame = CGRectMake(10.f, 0.0f, 150.f, 20.f);

    CGRect bodyFrame = CGRectMake(10.f, 20.f, CGRectGetWidth(contentRect)-20.f, CGRectGetHeight(contentRect)-20.f);

    timestamp.frame = timestampFrame;
    who.frame = whoFrame;
    body.frame = bodyFrame;
}

-(void)setMessage:(ChatMessage*)m
{
    message = m;
    timestamp.text = [dateFormatter stringFromDate:message.timestamp];
    who.text = message.who;
    body.text = message.text;    
}

@end

@interface ChatController()

@property (nonatomic) id<ChatChatSessionPrx> session;
@property (nonatomic) id<GLACIER2RouterPrx> router;
@property (nonatomic) id<ICECommunicator> communicator;
@property (nonatomic) id<ChatChatRoomCallbackPrx> callbackProxy;

@end

@implementation ChatController

@synthesize session;
@synthesize router;
@synthesize communicator;
@synthesize callbackProxy;

-(void)viewDidLoad
{
    messages = [NSMutableArray array];
    
    self.navigationItem.rightBarButtonItem =
    [[UIBarButtonItem alloc] initWithTitle:@"Users"
                                      style:UIBarButtonItemStylePlain
                                     target:self action:@selector(users:)];
    
    self.navigationItem.leftBarButtonItem =
    [[UIBarButtonItem alloc] initWithTitle:@"Logout"
                                      style:UIBarButtonItemStylePlain
                                     target:self action:@selector(logout:)];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(enterBackground)
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];
    
    userController = [[UserController alloc] initWithNibName:@"UserView" bundle:nil];
}

#pragma mark SessionManagement

-(void)enterBackground
{
    [self destroySession];
    [self.navigationController popViewControllerAnimated:NO];
}

-(void)destroySession
{
    self.session = nil;
    
    // Destroy the session and destroy the communicator from another thread since these
    // calls block.
    id<ICECommunicator> c = communicator;
    id<GLACIER2RouterPrx> r = router;
    self.router = nil;
    self.communicator = nil;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        @try
        {
            [r destroySession];
        }
        @catch (ICEException* ex) {
        }

        @try
        {            
            [c destroy];
        }
        @catch (ICEException* ex) {
        }
    });
}

-(void)exception:(ICEException*)ex
{
    // open an alert with just an OK button
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                     message:[ex description]
                                                    delegate:self
                                           cancelButtonTitle:@"OK"
                                           otherButtonTitles:nil];
    [alert show];       
    
    [self destroySession];
    
    [self.navigationController popViewControllerAnimated:YES];
}

// Called by the thread other than main.
-(void)  setup:(id<ICECommunicator>)c
       session:(id<ChatChatSessionPrx>)s
    acmTimeout:(ICEInt)t
        router:(id<GLACIER2RouterPrx>)r
      category:(NSString*)category
{
    self.communicator = c;
    self.session = s;
    self.router = r;
    acmTimeout = t;
    id<ICEObjectAdapter> adapter = [communicator createObjectAdapterWithRouter:@"ChatDemo.Client"
                                                                        router:router];
    [adapter activate];
    
    // Here we tie the chat view controller to the ChatRoomCallback servant.
    ChatChatRoomCallback* callbackImpl = [ChatChatRoomCallback objectWithDelegate:self];
    
    ICEIdentity* callbackId = [ICEIdentity identity:[ICEUtil generateUUID] category:category];

    // The callback is registered in clear:, otherwise the callbacks can arrive
    // prior to the IBOutlet connections being setup.
    self.callbackProxy = [ChatChatRoomCallbackPrx uncheckedCast:[adapter add:callbackImpl identity:callbackId]];
}

-(void)closed:(id<ICEConnection>)connection
{
    // The session is invalid, clear.
    if(self.session != nil)
    {
        [self.navigationController popToRootViewControllerAnimated:YES];
        
        // The session is invalid, clear.
        self.session = nil;
        self.router = nil;
        
        // Clean up the remainder.
        [self destroySession];
        
        // open an alert with just an OK button
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                        message:@"Lost connection with session!\n"
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
    }
}

// Called when the chat controller becomes active.
-(void)activate:(NSString*)t
{
    self.title = t;
    
    [messages removeAllObjects];
    [chatView reloadData];
    
    id<ICEConnection> conn = [router ice_getCachedConnection];
    id heartbeat = @(ICEHeartbeatAlways);
    id timeout = [NSNumber numberWithInteger:acmTimeout];
    [conn setACM:timeout close:ICENone heartbeat:heartbeat];
    [conn setCloseCallback:^(id<ICEConnection> connection) {
        [self closed:connection];
    }];

    
    // Register the chat callback.
    [session begin_setCallback:callbackProxy response:nil exception:^(ICEException* ex) { [self exception:ex]; }];
}

-(void)logout:(id)sender
{
    [self destroySession];
    [self.navigationController popViewControllerAnimated:YES];
}

#pragma mark UIViewController delegate methods

- (void)viewWillAppear:(BOOL)animated
{
    // Register for keyboard show/hide notifications.
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillShow:) 
                                                 name:UIKeyboardWillShowNotification
                                               object:self.view.window]; 
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillHide:) 
                                                 name:UIKeyboardWillHideNotification
                                               object:self.view.window]; 
}

- (void)viewWillDisappear:(BOOL)animated
{
    // Unregister for keyboard show/hide notifications.
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillShowNotification object:nil]; 
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillHideNotification object:nil]; 
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [inputField resignFirstResponder];
    inputField.text = @""; 
    [super touchesBegan:touches withEvent:event];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
	// Release anything that's not essential, such as cached data
}

#pragma mark -

-(void)append:(ChatMessage*)message
{
    if(messages.count > 100) // max 100 messages
    {
        [messages removeObjectAtIndex:0];
    }
    [messages addObject:message];
    [chatView reloadData];
    NSUInteger path[] = {0, messages.count-1};
    [chatView scrollToRowAtIndexPath:[NSIndexPath indexPathWithIndexes:path length:2] 
                    atScrollPosition:UITableViewScrollPositionBottom animated:NO];
}

-(void)users:(id)sender
{
    [self.navigationController pushViewController:userController animated:YES];    
}

- (void)setViewMovedUp:(BOOL)movedUp bounds:(CGRect)bounds
{
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:0.3];

    CGRect rect = self.view.frame;
    if(movedUp)
    {
        rect.origin.y -= CGRectGetHeight(bounds);
    }
    else
    {
        rect.origin.y += CGRectGetHeight(bounds);
    }
    self.view.frame = rect;
    
    [UIView commitAnimations];
}

#pragma mark Keyboard notifications

- (void)keyboardWillShow:(NSNotification *)notif
{
    CGRect r;
    [[[notif userInfo] objectForKey:UIKeyboardFrameBeginUserInfoKey] getValue:&r];
    [self setViewMovedUp:YES bounds:r];
}

- (void)keyboardWillHide:(NSNotification *)notif
{
    CGRect r;
    [[[notif userInfo] objectForKey:UIKeyboardFrameEndUserInfoKey] getValue:&r];
    [self setViewMovedUp:NO bounds:r];
}

#pragma mark UITextFieldDelegate

-(BOOL)textFieldShouldReturn:(UITextField*)theTextField
{    
    NSString* s  = [theTextField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    if(s.length > 1024)
    {
        s = [s substringToIndex:1024];
    }
    
    NSAssert(s.length <= 1024, @"s.length <= 1024");
    if(s.length > 0)
    {
        [session begin_send:s response:^(ICELong t) { } exception:^(ICEException* ex) { [self exception:ex]; }];
    }
    
    theTextField.text = @"";
    [theTextField resignFirstResponder];
    
    return YES;
}

#pragma mark ChatRoomCallbck

-(void)init:(NSMutableArray *)users current:(ICECurrent*)current;
{
    userController.users = users;
    [userController.usersTableView reloadData];
    self.navigationItem.rightBarButtonItem.title = [NSString stringWithFormat:@"%lu %@",
                                                    (unsigned long)userController.users.count,
                                                    (userController.users.count > 1 ? @"users" : @"user")];
}

-(void)send:(ICELong)timestamp name:(NSMutableString *)name message:(NSMutableString *)message current:(ICECurrent*)current;
{
    [self append:[ChatMessage chatMessageWithText:message who:name timestamp:timestamp]];
}

-(void)join:(ICELong)timestamp name:(NSMutableString*)name current:(ICECurrent*)current;
{
    NSString* s = [NSString stringWithFormat:@"%@ joined.\n", name];
    [self append:[ChatMessage chatMessageWithText:s who:@"system message" timestamp:timestamp]];

    [userController.users addObject:name];
    [userController.usersTableView reloadData];
    
    self.navigationItem.rightBarButtonItem.title = [NSString stringWithFormat:@"%lu %@",
                                                    (unsigned long)userController.users.count,
                                                    (userController.users.count > 1 ? @"users" : @"user")];
}

-(void)leave:(ICELong)timestamp name:(NSMutableString*)name current:(ICECurrent*)current;
{
    NSString* s = [NSString stringWithFormat:@"%@ left.\n", name];
    [self append:[ChatMessage chatMessageWithText:s who:@"system message" timestamp:timestamp]];
    
    NSUInteger index = [userController.users indexOfObject:name];
    if(index != NSNotFound)
    {
        [userController.users removeObjectAtIndex:index];
        [userController.usersTableView reloadData];
    }

    self.navigationItem.rightBarButtonItem.title = [NSString stringWithFormat:@"%lu %@",
                                                    (unsigned long)userController.users.count,
                                                    (userController.users.count > 1 ? @"users" : @"user")];
}

#pragma mark <UITableViewDelegate, UITableViewDataSource> Methods

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tv
{
    return 1;
}

-(NSInteger)tableView:(UITableView *)tv numberOfRowsInSection:(NSInteger)section
{
    return messages.count;
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return chatView.sectionHeaderHeight;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return [MessageCell heightForMessage:[messages objectAtIndex:indexPath.row]];
}

-(UITableViewCell *)tableView:(UITableView *)tv cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    MessageCell *cell = (MessageCell*)[chatView dequeueReusableCellWithIdentifier:@"MessageCell"];
    if(cell == nil)
    {
        // Create a new cell. CGRectZero allows the cell to determine the appropriate size.
        cell = [[MessageCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"MessageCell"];
    }
    cell.message = [messages objectAtIndex:indexPath.row];
    return cell;
}

-(NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    return nil;
}

@end
