// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************


#import <UIKit/UIKit.h>
#import <LoggingDelegate.h>

@class LogZoomViewController;

@interface LogViewController : UIViewController<LoggingDelegate, UITableViewDelegate, UITableViewDataSource>
{
    IBOutlet UITableView* messagesTableView;
    NSMutableArray* messages;
    LogZoomViewController* logZoomViewController;
}

@property (nonatomic) UITableView* messagesTableView;
@property (nonatomic) NSMutableArray* messages;
@property (nonatomic) LogZoomViewController* logZoomViewController;

@end
