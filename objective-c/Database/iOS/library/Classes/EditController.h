// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>

@interface EditController : UIViewController

@property (nonatomic) IBOutlet UITextField* textField;
@property (nonatomic) NSString* initialValue;

-(void)startEdit:(NSString*)name value:(NSString*)value;

@end
