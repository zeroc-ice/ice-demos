// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>
#import <objc/Ice.h>

#import <DetailController.h>

@protocol DemoLibraryPrx;
@protocol DemoBookQueryResultPrx;
@protocol GLACIER2RouterPrx;

@class AddController;

@interface MainController :
    UIViewController<UISearchBarDelegate, UITableViewDataSource, UITableViewDelegate,UIAlertViewDelegate,
                     DetailControllerDelegate>
{
@private
    IBOutlet UISegmentedControl* searchSegmentedControl;
    IBOutlet UITableView* searchTableView;

    NSIndexPath* currentIndexPath;

    DetailController* detailController;
    AddController* addController;
    id<DemoBookQueryResultPrx> query;
    NSMutableArray* books;
    int rowsQueried;
    int nrows;

    // Session state.
    NSTimer* refreshTimer;
    id<ICECommunicator> communicator;
    id session;
    id<GLACIER2RouterPrx> router;
    id<DemoLibraryPrx> library;
}

// Called to setup the session.
-(void)activate:(id<ICECommunicator>)communicator
        session:(id)session
         router:(id<GLACIER2RouterPrx>)router
 sessionTimeout:(ICELong)timeout
        library:(id<DemoLibraryPrx>)library;

@end
