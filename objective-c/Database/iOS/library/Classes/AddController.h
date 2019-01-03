// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>
#import <DetailController.h>

@protocol DemoLibraryPrx;

@class WaitAlert;

@interface AddController : DetailController
{
@private

    id<DemoLibraryPrx> library;
}

-(IBAction)save:(id)sender;

-(void)startEdit:(DemoBookDescription*)book library:(id<DemoLibraryPrx>)library;

@end
