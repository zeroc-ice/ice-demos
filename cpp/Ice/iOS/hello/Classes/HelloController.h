// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <UIKit/UIKit.h>

@protocol ICECommunicator;

@interface HelloController : UIViewController<UITextFieldDelegate, UIAlertViewDelegate, UIPickerViewDataSource,
                                                  UIPickerViewDelegate >
{
@private
    IBOutlet UIButton* sayHelloButton;
    IBOutlet UIButton* shutdownButton;
    IBOutlet UIButton* flushButton;
    IBOutlet UISwitch* useDiscovery;
    IBOutlet UITextField* hostnameTextField;
    IBOutlet UILabel* statusLabel;
    IBOutlet UIPickerView* modePicker;
    IBOutlet UISlider* timeoutSlider;
    IBOutlet UISlider* delaySlider;
    IBOutlet UIActivityIndicatorView* activity;

    BOOL showAlert;
    id<ICECommunicator> communicator;
}

- (void)flushBatchSend;
- (void)requestSent:(int)deliveryMode hostname:(NSString*)hostname;
- (void)sendingRequest;
- (void)ready;
- (void)queuedRequest:(NSString*)name;
- (IBAction)sayHello:(id)sender;
- (IBAction)flushBatch:(id) sender;
- (IBAction)shutdown: (id)sender;
- (IBAction)useDiscovery:(id)sender;
- (void)exception:(NSString*)s;

@end

