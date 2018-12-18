// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <HelloController.h>
#import <objc/Ice.h>
#import <Hello.h>

// Various delivery mode constants
#define DeliveryModeTwoway  0
#define DeliveryModeTwowaySecure 1
#define DeliveryModeOneway 2
#define DeliveryModeOnewayBatch  3
#define DeliveryModeOnewaySecure 4
#define DeliveryModeOnewaySecureBatch 5
#define DeliveryModeDatagram 6
#define DeliveryModeDatagramBatch 7

//
// Avoid warning for undocumented UISlider method
//
@interface UISlider(UndocumentedAPI)
-(void)setShowValue:(BOOL)val;
@end

@interface HelloController()
@property (nonatomic) UILabel* statusLabel;
@end

@implementation HelloController

@synthesize statusLabel;

static NSString* hostnameKey = @"hostnameKey";

+(void)initialize
{
    NSDictionary* appDefaults = [NSDictionary dictionaryWithObjectsAndKeys:@"127.0.0.1", hostnameKey, nil];

    [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
}
-(void)applicationWillTerminate
{
    [communicator destroy];
}
-(void)viewDidLoad
{
    ICEregisterIceDiscovery(YES);
    ICEregisterIceSSL(YES);
    ICEregisterIceUDP(YES);
    ICEregisterIceIAP(YES);

    ICEInitializationData* initData = [ICEInitializationData initializationData];
    initData.properties = [ICEUtil createProperties];
    [initData.properties setProperty:@"IceSSL.CheckCertName" value:@"0"];
    [initData.properties setProperty:@"IceSSL.CAs" value:@"cacert.der"];
    [initData.properties setProperty:@"IceSSL.CertFile" value:@"client.p12"];
    [initData.properties setProperty:@"IceSSL.Password" value:@"password"];

    // Dispatch AMI callbacks on the main thread
    initData.dispatcher = ^(id<ICEDispatcherCall> call, id<ICEConnection> con)
    {
        dispatch_sync(dispatch_get_main_queue(), ^ { [call run]; });
    };
    communicator = [ICEUtil createCommunicator:initData];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationWillTerminate)
                                                 name:UIApplicationWillTerminateNotification
                                               object:nil];

    // When the user starts typing, show the clear button in the text field.
    hostnameTextField.clearButtonMode = UITextFieldViewModeWhileEditing;

    // Defaults for the UI elements.
    hostnameTextField.text = [[NSUserDefaults standardUserDefaults] stringForKey:hostnameKey];
    flushButton.enabled = NO;
    [flushButton setAlpha:0.5];
    [useDiscovery setOn:NO];

    // This generates a compile time warning, but does actually work!
    [delaySlider setShowValue:YES];
    [timeoutSlider setShowValue:YES];

    statusLabel.text = @"Ready";
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

#pragma mark UITextFieldDelegate

-(BOOL)textFieldShouldReturn:(UITextField *)theTextField
{
    // Close the text field.
    [theTextField resignFirstResponder];
    return YES;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Dismiss the keyboard when the view outside the text field is touched.
    [hostnameTextField resignFirstResponder];

    [super touchesBegan:touches withEvent:event];
}

#pragma mark AMI Callbacks

-(void)sendingRequest
{
    sayHelloButton.enabled = NO;
    shutdownButton.enabled = NO;
    statusLabel.text = @"Sending request";
    [activity startAnimating];
}

-(void) requestSent
{
    if([useDiscovery isOn])
    {
        id<ICEConnection> connection = [helloPrx ice_getCachedConnection];
        if(connection != nil)
        {
            @try
            {
                // Loop through the connection informations until we find an IPConnectionInfo class.
                for(ICEConnectionInfo* info = [connection getInfo]; info; info = info.underlying)
                {
                    if([info isKindOfClass:[ICEIPConnectionInfo class]])
                    {
                        hostnameTextField.text = ((ICEIPConnectionInfo*)info).remoteAddress;
                        [[NSUserDefaults standardUserDefaults] setObject:hostnameTextField.text forKey:hostnameKey];
                        break;
                    }
                }
            }
            @catch(ICELocalException* ex)
            {
                // Ignore.
            }
        }
    }
    if(deliveryMode == DeliveryModeTwoway || deliveryMode == DeliveryModeTwowaySecure)
    {
        statusLabel.text = @"Waiting for response";
    }
    else
    {
        [self ready];
    }
}

-(void)ready
{
    sayHelloButton.enabled = YES;
    shutdownButton.enabled = YES;
    statusLabel.text = @"Ready";
    [activity stopAnimating];
}

-(void)queuedRequest:(NSString*)name
{
    flushButton.enabled = YES;
    [flushButton setAlpha:1.0];
    statusLabel.text = [NSString stringWithFormat:@"Queued %@ request", name];
}

-(void)flushBatchSend
{
    flushButton.enabled = NO;
    [flushButton setAlpha:0.5];
    statusLabel.text = @"Flushed batch requests";
}

-(void)exception:(ICEException*)ex
{
    NSString* s = [NSString stringWithFormat:@"%@", ex];
    // open an alert with just an OK button
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Error"
                                                                   message:s
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];
    [self presentViewController:alert animated:YES completion:nil];
    [self ready];
}

#pragma mark UI Element Callbacks

-(void) updateProxy
{
    int t = (int)(timeoutSlider.value * 1000.0f); // Convert to ms.
    if(helloPrx != nil &&
       [hostnameTextField.text isEqualToString:hostname] &&
       timeout == t &&
       deliveryMode == [modePicker selectedRowInComponent:0] &&
       discovery == [useDiscovery isOn])
    {
        return;
    }

    deliveryMode = [modePicker selectedRowInComponent:0];
    timeout = t;
    hostname = hostnameTextField.text;
    discovery = [useDiscovery isOn];

    if(!discovery && [hostname isEqualToString:@""])
    {
        helloPrx = nil;
        return;
    }

    NSString* s;
    if(discovery)
    {
        s = @"hello";
    }
    else
    {
        s = [NSString stringWithFormat:@"hello:tcp -h \"%@\" -p 10000:ssl -h \"%@\" -p 10001:udp -h \"%@\" -p 10000",
                                       hostname, hostname, hostname];
    }

    [[NSUserDefaults standardUserDefaults] setObject:hostname forKey:hostnameKey];
    ICEObjectPrx* prx = [communicator stringToProxy:s];
    switch(deliveryMode)
    {
        case DeliveryModeTwoway:
            prx = [prx ice_twoway];
            break;
        case DeliveryModeTwowaySecure:
            prx = [[prx ice_twoway] ice_secure:YES];
            break;
        case DeliveryModeOneway:
            prx = [prx ice_oneway];
            break;
        case DeliveryModeOnewayBatch:
            prx = [prx ice_batchOneway];
            break;
        case DeliveryModeOnewaySecure:
            prx = [[prx ice_oneway] ice_secure:YES];
            break;
        case DeliveryModeOnewaySecureBatch:
            prx = [[prx ice_batchOneway] ice_secure:YES];
            break;
        case DeliveryModeDatagram:
            prx = [prx ice_datagram];
            break;
        case DeliveryModeDatagramBatch:
            prx = [prx ice_batchDatagram];
            break;
    }

    if(timeout != 0)
    {
        prx = [prx ice_invocationTimeout:timeout];
    }

    helloPrx = [DemoHelloPrx uncheckedCast:prx];
}

-(IBAction)sayHello:(id)sender
{
    @try
    {
        [self updateProxy];
        if(helloPrx == nil)
        {
            return;
        }

        int delay = (int)(delaySlider.value * 1000.0f); // Convert to ms.
        if(deliveryMode != DeliveryModeOnewayBatch &&
           deliveryMode != DeliveryModeOnewaySecureBatch &&
           deliveryMode != DeliveryModeDatagramBatch)
        {
            [self sendingRequest];
            __block BOOL response = NO;
            [helloPrx begin_sayHello:delay
                            response:^ {
                                response = YES;
                                [self ready];
                            }
                           exception:^(ICEException* ex) {
                               response = YES;
                               [self exception:ex];
                           }
                                sent:^(BOOL sentSynchronously) {
                                    if(response)
                                    {
                                        return;
                                    }
                                    [self requestSent];
                                }];
        }
        else
        {
            [helloPrx sayHello:delay];
            [self queuedRequest:@"hello"];
        }
    }
    @catch(ICELocalException* ex)
    {
        [self exception:ex];
    }
}
-(IBAction)useDiscovery:(id)sender
{
    if([useDiscovery isOn])
    {
        [hostnameTextField setEnabled:NO];
        hostnameTextField.text = @"";
    }
    else
    {
        [hostnameTextField setEnabled:YES];
        hostnameTextField.text = [[NSUserDefaults standardUserDefaults] stringForKey:hostnameKey];
    }
}
-(IBAction)shutdown:(id)sender
{
    if(helloPrx == nil)
    {
        return;
    }

    @try
    {
        [self updateProxy];
        if(helloPrx == nil)
        {
            return;
        }

        if(deliveryMode != DeliveryModeOnewayBatch &&
           deliveryMode != DeliveryModeOnewaySecureBatch &&
           deliveryMode != DeliveryModeDatagramBatch)
        {
            [self sendingRequest];
            __block BOOL response = NO;
            [helloPrx begin_shutdown:^ {
                                response = YES;
                                [self ready];
                            }
                           exception:^(ICEException* ex) {
                               response = YES;
                               [self exception:ex];
                           }
                                sent:^(BOOL sentSynchronously) {
                                    if(response)
                                    {
                                        return;
                                    }
                                    [self requestSent];
                                }];
        }
        else
        {
            [helloPrx shutdown];
            [self queuedRequest:@"shutdown"];
        }
    }
    @catch(ICELocalException* ex)
    {
        [self exception:ex];
    }
}

-(IBAction)flushBatch:(id) sender
{
    if(helloPrx == nil)
    {
        return;
    }

    @try
    {
        flushButton.enabled = NO;
        [flushButton setAlpha:0.5];
        [helloPrx begin_ice_flushBatchRequests:^(ICEException* ex) {
                                                  [self exception:ex];
                                               }
                                               sent:^(BOOL sentSynchronously) {
                                                  self.statusLabel.text = @"Flushed batch requests";
                                               }];
    }
    @catch(ICELocalException* ex)
    {
        [self exception:ex];
    }
}

#pragma mark UIPickerViewDataSource

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return 8;
}

#pragma mark UIPickerViewDelegate

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    switch(row)
    {
        case DeliveryModeTwoway:
            return @"Twoway";
        case DeliveryModeTwowaySecure:
            return @"Twoway secure";
        case DeliveryModeOneway:
            return @"Oneway";
        case DeliveryModeOnewayBatch:
            return @"Oneway batch";
        case DeliveryModeOnewaySecure:
            return @"Oneway secure";
        case DeliveryModeOnewaySecureBatch:
            return @"Oneway secure batch";
        case DeliveryModeDatagram:
            return @"Datagram";
        case DeliveryModeDatagramBatch:
            return @"Datagram batch";
    }
    return @"UNKNOWN";
}

@end
