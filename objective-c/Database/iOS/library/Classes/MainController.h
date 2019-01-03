// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>
#import <objc/Ice.h>

#import <DetailController.h>

@protocol DemoLibraryPrx;
@protocol DemoBookQueryResultPrx;
@protocol GLACIER2RouterPrx;

@interface MainController :
    UIViewController<UISearchBarDelegate, UITableViewDataSource, UITableViewDelegate,UIAlertViewDelegate,
                     DetailControllerDelegate>
{
@private
    IBOutlet UITableView* searchTableView;

    NSIndexPath* currentIndexPath;

    id<DemoBookQueryResultPrx> query;
    NSMutableArray* books;
    int rowsQueried;
    int nrows;

    // Session state.
    id<ICECommunicator> communicator;
    id session;
    id<GLACIER2RouterPrx> router;
    id<DemoLibraryPrx> library;
}

// Called to setup the session.
-(void)activate:(id<ICECommunicator>)communicator
        session:(id)session
         router:(id<GLACIER2RouterPrx>)router
        library:(id<DemoLibraryPrx>)library;

@end
