// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>

#import <Voip.h>

@class ICEInitializationData;
@protocol ICECommunicator;
@protocol GLACIER2RouterPrx;

@interface VoipViewController : UIViewController<UITextFieldDelegate,VoipControl> {
@private
    IBOutlet UITextField* usernameField;
    IBOutlet UITextField* passwordField;
    IBOutlet UIButton* loginButton;
	IBOutlet UIButton* callButton;
    IBOutlet UISlider* delaySlider;
    IBOutlet UILabel* statusLabel;
    IBOutlet UIActivityIndicatorView* statusActivity;
	
    UITextField* currentField;
    NSString* oldFieldValue;

    // Per session state.
    NSTimer* refreshTimer;
    id<ICECommunicator> communicator;
	id<VoipSessionPrx> session;
	id<GLACIER2RouterPrx> router;
	ICELong sessionTimeout;
}

-(IBAction)login:(id)sender;
-(IBAction)call:(id)sender;

@end

