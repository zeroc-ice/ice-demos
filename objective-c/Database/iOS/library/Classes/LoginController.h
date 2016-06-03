// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>
#import <objc/Ice.h>

@protocol DemoLibraryPrx;
@class MainController;
@class WaitAlert;
@protocol GLACIER2RouterPrx;

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
    
    MainController *mainController;

    WaitAlert* waitAlert;
    
    id<ICECommunicator> communicator;
    id session;
    id<GLACIER2RouterPrx> router;
    ICELong sessionTimeout;
    id<DemoLibraryPrx> library;
}

-(IBAction)login:(id)sender;

@end
