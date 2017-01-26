// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>

@class ChatController;
@class WaitAlert;
@class ICEInitializationData;
@protocol ICECommunicator;

@interface LoginController : UIViewController<UITextFieldDelegate,UIAlertViewDelegate>
{
@private
    IBOutlet UITextField* usernameField;
    IBOutlet UITextField* passwordField;
    IBOutlet UIButton* loginButton;
    IBOutlet UILabel* statusLabel;
    IBOutlet UIActivityIndicatorView* statusActivity;

    UITextField* currentField;
    NSString* oldFieldValue;
    ChatController* chatController;
    WaitAlert* waitAlert;

    id<ICECommunicator> communicator;
}

-(IBAction)login:(id)sender;

@end
