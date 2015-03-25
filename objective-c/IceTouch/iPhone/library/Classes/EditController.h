// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
//
// **********************************************************************

#import <UIKit/UIKit.h>


@interface EditController : UIViewController
{
@private
    
    IBOutlet UITextField* __unsafe_unretained textField;
    
    id obj;
    SEL selector;
    NSString* value;
}

@property (unsafe_unretained, nonatomic, readonly) UITextField *textField;

-(IBAction)cancel:(id)sender;
-(IBAction)save:(id)sender;
-(void)startEdit:(id)obj selector:(SEL)selector name:(NSString*)name value:(NSString*)value;

@end
