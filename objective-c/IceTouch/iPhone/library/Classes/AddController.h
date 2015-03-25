// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
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


-(IBAction)cancel:(id)sender;
-(IBAction)save:(id)sender;

-(void)startEdit:(DemoBookDescription*)book library:(id<DemoLibraryPrx>)library;

@end
