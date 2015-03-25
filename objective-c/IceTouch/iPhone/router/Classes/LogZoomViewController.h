// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
//
// **********************************************************************

#import <UIKit/UIKit.h>


@interface LogZoomViewController : UIViewController {
    UITextView* text;
    NSArray* messages;
    int current;
    UIButton* nextButton;
    UIButton* prevButton;
    NSDateFormatter *dateFormatter;
}

@property (nonatomic) IBOutlet UITextView* text;
@property (nonatomic) IBOutlet UIButton* nextButton;
@property (nonatomic) IBOutlet UIButton* prevButton;

@property (nonatomic) NSArray* messages;
@property (nonatomic) int current;

@property (nonatomic) NSDateFormatter* dateFormatter;

-(IBAction)next:(id)sender;
-(IBAction)prev:(id)sender;

@end
