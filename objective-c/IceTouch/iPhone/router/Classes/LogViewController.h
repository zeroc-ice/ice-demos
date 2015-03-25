// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
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
