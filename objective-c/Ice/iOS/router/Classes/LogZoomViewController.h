// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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
