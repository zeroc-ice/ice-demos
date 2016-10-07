This demo illustrates how to write an iPhone background application.
The demo only simulates how Ice might be be used for the control
channel of a VoIP application.

ZeroC hosts a server for this demo on demo2.zeroc.com. The iPhone
client connects to this server though a WebSocket proxy hosted 
on zeroc.com.

After pressing the "Call Back" button the server will call
"incomingCall" on the `Voip::Control` callback proxy after the specified
delay. A local notification is pushed if the application is in the
background, otherwise an alert box is displayed.

Note that VoIP sockets are no longer supported with iOS 10. The `Ice.Voip`
property used by this demo will be deprecated in upcoming releases. Applications
are encouraged to use the iOS PushKit API instead.

If you want to deploy your own VoIP server, you can run the
[Java voip demo server](../../../../java/Glacier2/voip).
