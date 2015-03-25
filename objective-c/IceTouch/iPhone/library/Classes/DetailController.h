// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
//
// **********************************************************************

#import <UIKit/UIKit.h>

@class DemoBookDescription;
@class ICEException;
@class WaitAlert;

@protocol DetailControllerDelegate
-(void)bookUpdated:(DemoBookDescription*)book;
-(void)bookDeleted;
-(void)destroySession;
@end

@interface DetailController :
    UIViewController<UITableViewDelegate, UITableViewDataSource, UIActionSheetDelegate, UIAlertViewDelegate>
{
@protected
    DemoBookDescription* book;
    DemoBookDescription* updated;
    
    IBOutlet UITableView *tableView;
    IBOutlet UIActivityIndicatorView *statusActivity;
    IBOutlet UILabel *statusLabel;
    NSIndexPath* selectedIndexPath;
    WaitAlert* waitAlert;
    
    id<DetailControllerDelegate> delegate;
    bool saving;
    bool changed;
    bool fatal;
}

@property (nonatomic) DemoBookDescription* book;
@property (nonatomic) WaitAlert* waitAlert;
@property (nonatomic) id<DetailControllerDelegate> delegate;

-(void)saving:(BOOL)saving;
-(void)exception:(ICEException*)ex;
-(void)startEdit:(DemoBookDescription*)book;

@end
