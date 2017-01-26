// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <HelloController.h>
#import <Ice/Ice.h>
#import <Hello.h>

#import <UIKit/UIKit.h>

#include <iostream>
#include <string>

using namespace Demo;

// Various delivery mode constants
#define DeliveryModeTwoway  0
#define DeliveryModeTwowaySecure 1
#define DeliveryModeOneway 2
#define DeliveryModeOnewayBatch  3
#define DeliveryModeOnewaySecure 4
#define DeliveryModeOnewaySecureBatch 5
#define DeliveryModeDatagram 6
#define DeliveryModeDatagramBatch 7

using namespace std;

namespace
{

class Dispatcher : public Ice::Dispatcher
{
public:

    virtual void
    dispatch(const Ice::DispatcherCallPtr& call, const Ice::ConnectionPtr&)
    {
        dispatch_sync(dispatch_get_main_queue(), ^ {
                @autoreleasepool
                {
                    call->run();
                }
            });
    }
};

class HelloClient : public IceUtil::Shared
{
public:

    HelloClient(HelloController* controller) :
        _controller(controller), _response(false)
    {
        Ice::registerIceDiscovery(false); // Register the plugin but don't load it on initialization
        Ice::registerIceSSL();
        
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        initData.properties->setProperty("IceSSL.CheckCertName", "0");
        initData.properties->setProperty("IceSSL.CAs", "cacert.der");
        initData.properties->setProperty("IceSSL.CertFile", "client.p12");
        initData.properties->setProperty("IceSSL.Password", "password");
        initData.properties->setProperty("Ice.Plugin.IceDiscovery", "1");
        initData.dispatcher = new Dispatcher();
        try
        {
            _communicator = Ice::initialize(initData);
        }
        catch(const Ice::PluginInitializationException& ex)
        {
            //
            // IceDiscovery might fail to join the multicast group if there's no network supporting
            // multicast, disable it and try again.
            //
            initData.properties->setProperty("Ice.Plugin.IceDiscovery", "");
            _communicator = Ice::initialize(initData);
        }
    }

    void updateProxy(const string& hostname, int deliveryMode, int timeout, bool discovery)
    {
        if(_helloPrx &&
           _hostname == hostname &&
           deliveryMode == _deliveryMode &&
           timeout == _timeout &&
           discovery == _discovery)
        {
            return;
        }

        _deliveryMode = deliveryMode;
        _hostname = hostname;
        _timeout = timeout;
        _discovery = discovery;

        if(_hostname.empty() && !_discovery)
        {
            return;
        }

        ostringstream os;
        os << "hello";
        if(!_discovery)
        {
            os << ":tcp -h \"" << _hostname << "\" -p 10000";
            os << ":ssl -h \"" << _hostname << "\" -p 10001";
            os << ":udp -h \"" << _hostname << "\" -p 10000";
        }

        _helloPrx = HelloPrx::uncheckedCast(_communicator->stringToProxy(os.str()));
        switch(deliveryMode)
        {
            case DeliveryModeTwoway:
                _helloPrx = _helloPrx->ice_twoway();
                break;
            case DeliveryModeTwowaySecure:
                _helloPrx = _helloPrx->ice_twoway()->ice_secure(true);
                break;
            case DeliveryModeOneway:
                _helloPrx = _helloPrx->ice_oneway();
                break;
            case DeliveryModeOnewayBatch:
                _helloPrx = _helloPrx->ice_batchOneway();
                break;
            case DeliveryModeOnewaySecure:
                _helloPrx = _helloPrx->ice_oneway()->ice_secure(true);
                break;
            case DeliveryModeOnewaySecureBatch:
                _helloPrx = _helloPrx->ice_batchOneway()->ice_secure(true);
                break;
            case DeliveryModeDatagram:
                _helloPrx = _helloPrx->ice_datagram();
                break;
            case DeliveryModeDatagramBatch:
                _helloPrx = _helloPrx->ice_batchDatagram();
                break;
        }

        if(_timeout != 0)
        {
            _helloPrx = _helloPrx->ice_invocationTimeout(_timeout);
        }
    }

    void sayHello(const string& hostname, int deliveryMode, int timeout, int delay, bool discovery)
    {
        try
        {
            updateProxy(hostname, deliveryMode, timeout, discovery);
            if(!_helloPrx)
            {
                return;
            }

            if(deliveryMode != DeliveryModeOnewayBatch &&
               deliveryMode != DeliveryModeOnewaySecureBatch &&
               deliveryMode != DeliveryModeDatagramBatch)
            {
                _response = false;
                [_controller sendingRequest];
                _helloPrx->begin_sayHello(delay, newCallback_Hello_sayHello(this,
                                                                            &HelloClient::response,
                                                                            &HelloClient::exception,
                                                                            &HelloClient::sent));
            }
            else
            {
                _helloPrx->sayHello(delay);
                [_controller queuedRequest:@"hello"];
            }
        }
        catch(const Ice::LocalException& ex)
        {
            exception(ex);
        }
    }

    void response()
    {
        _response = true;
        [_controller ready];
    }

    void sent(bool sentSynchronously)
    {
        if(_response)
        {
            return;
        }

        NSString* hostname = nil;
        Ice::ConnectionPtr connection = _helloPrx->ice_getCachedConnection();
        if(connection)
        {
            try
            {
                // Loop through the connection informations until we find an IPConnectionInfo class.
                for(Ice::ConnectionInfoPtr info = connection->getInfo(); info; info = info->underlying)
                {
                    Ice::IPConnectionInfoPtr ipinfo = Ice::IPConnectionInfoPtr::dynamicCast(info);
                    if(ipinfo)
                    {
                        hostname = [NSString stringWithUTF8String:ipinfo->remoteAddress.c_str()];
                        break;
                    }
                }
            }
            catch(const Ice::LocalException&)
            {
                // Ignore.
            }
        }
        [_controller requestSent:_deliveryMode hostname:hostname];
    }

    void exception(const Ice::Exception& ex)
    {
        _response = true;

        ostringstream os;
        os << ex;
        const string s = os.str();
        NSString* err = [NSString stringWithUTF8String:s.c_str()];
        [_controller exception:err];
    }

    void shutdown(const string& hostname, int deliveryMode, int timeout, bool discovery)
    {
        try
        {
            updateProxy(hostname, deliveryMode, timeout, discovery);
            if(!_helloPrx)
            {
                return;
            }

            if(deliveryMode != DeliveryModeOnewayBatch &&
               deliveryMode != DeliveryModeOnewaySecureBatch &&
               deliveryMode != DeliveryModeDatagramBatch)
            {
                _response = false;
                [_controller sendingRequest];
                _helloPrx->begin_shutdown(newCallback_Hello_shutdown(this,
                                                                     &HelloClient::response,
                                                                     &HelloClient::exception,
                                                                     &HelloClient::sent));
            }
            else
            {
                _helloPrx->shutdown();
                [_controller queuedRequest:@"shutdown"];
            }
        }
        catch(const Ice::LocalException& ex)
        {
            exception(ex);
        }
    }

    void flushBatchSend(bool)
    {
        [_controller flushBatchSend];
    }

    void flushBatch()
    {
        if(!_helloPrx)
        {
            return;
        }

        try
        {
            _helloPrx->begin_ice_flushBatchRequests(
                Ice::newCallback_Object_ice_flushBatchRequests(this,
                                                               &HelloClient::exception,
                                                               &HelloClient::flushBatchSend));
        }
        catch(const Ice::LocalException& ex)
        {
            exception(ex);
        }
    }

    void destroy()
    {
        if(_communicator)
        {
            _communicator->destroy();
            _communicator = 0;
        }
    }

private:

    IceUtil::Mutex _mutex;
    HelloController* _controller;
    Ice::CommunicatorPtr _communicator;
    bool _response;
    HelloPrx _helloPrx;
    int _deliveryMode;
    string _hostname;
    int _timeout;
    bool _discovery;
};

typedef IceUtil::Handle<HelloClient> HelloClientPtr;
HelloClientPtr client;

}

//
// Avoid warning for undocumented UISlider method
//
@interface UISlider(UndocumentedAPI)
-(void)setShowValue:(BOOL)val;
@end

@implementation HelloController

static NSString* hostnameKey = @"hostnameKey";

+(void)initialize
{
    NSDictionary* appDefaults = [NSDictionary dictionaryWithObjectsAndKeys:@"127.0.0.1", hostnameKey, nil];

    [[NSUserDefaults standardUserDefaults] registerDefaults:appDefaults];
}

-(void)applicationWillTerminate
{
    if(client)
    {
        client->destroy();
    }
}

-(void)viewDidLoad
{
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

    client = new HelloClient(self);

    showAlert = NO;
}

-(void)requestSent:(int)deliveryMode hostname:(NSString*)hostname
{
    if([useDiscovery isOn] && hostname != nil)
    {
        hostnameTextField.text = hostname;
        [[NSUserDefaults standardUserDefaults] setObject:hostnameTextField.text forKey:hostnameKey];
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

-(void)sendingRequest
{
    sayHelloButton.enabled = NO;
    shutdownButton.enabled = NO;
    statusLabel.text = @"Sending request";
    [activity startAnimating];
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


-(IBAction)sayHello:(id)sender
{
    client->sayHello([hostnameTextField.text cStringUsingEncoding:[NSString defaultCStringEncoding]],
                     (int)[modePicker selectedRowInComponent:0],
                     (int)(timeoutSlider.value * 1000.0f),     // Convert to ms.
                     (int)(delaySlider.value * 1000.0f),
                     [useDiscovery isOn]);  // Convert to ms.

}

-(IBAction)flushBatch:(id)sender
{
    client->flushBatch();
}

-(IBAction)shutdown:(id)sender
{
    client->shutdown([hostnameTextField.text cStringUsingEncoding:[NSString defaultCStringEncoding]],
                     (int)[modePicker selectedRowInComponent:0],
                     (int)(delaySlider.value * 1000.0f),
                     [useDiscovery isOn]);  // Convert to ms.

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

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

-(void)dealloc
{
    [sayHelloButton release];
    [shutdownButton release];
    [flushButton release];
    [hostnameTextField release];
    [statusLabel release];
    [timeoutSlider release];
    [delaySlider release];
    [activity release];
    [modePicker release];
    [super dealloc];
}

#pragma mark UIAlertViewDelegate

-(void)didPresentAlertView:(UIAlertView *)alertView
{
    showAlert = YES;
}

-(void)alertView:(UIAlertView*)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    showAlert = NO;
}

#pragma mark UITextFieldDelegate

-(BOOL)textFieldShouldReturn:(UITextField *)theTextField
{
    // If we've already showing an invalid hostname alert, then we ignore enter.
    if(showAlert)
    {
        return NO;
    }

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

-(void)exception:(NSString*)s
{
    // open an alert with just an OK button
    UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Error"
                                                     message:s
                                                        delegate:self
                                           cancelButtonTitle:@"OK"
                                           otherButtonTitles:nil] autorelease];

    [alert show];
    [self ready];
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
