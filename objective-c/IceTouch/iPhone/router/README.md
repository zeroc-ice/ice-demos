This demo illustrates how to route Ice requests from computers on an
isolated (not connected to the internet) WiFi network to the internet
using the cellular network of an iPhone.

The demo shows:

  - How to implement an Ice router. The Ice router implementation runs
    on an iPhone. Clients on the WiFi network establish a session
    with this router in order to be able to send Ice requests on the
    internet through the iPhone's celluar network.

  - How to implement a custom Ice logger.

To test the router provided with this demo, you can use the
[Cocoa library client](../../Cocoa/library), or the command line
[Objective-C helloRouter](../../../Ice/helloRouter) demo. These
clients can optionally connect to the libray or hello world servers
using the iPhone router provided with this demo.

You also need to configure the WiFi network appropriately on the
iPhone and computers that wish to route Ice requests through the
iPhone's cellular connection. The instructions below provide steps to
configure the iPhone and a OS X computer whose WiFi device is set up
to create the WiFi network (no WiFi router is required).

On the OS X computer, start System Preferences and choose
"Network". Select the Airport interface and in the "Network Name"
drop-down list, select "Create Network..." Enter a name such as
"iPhone Network". This creates a new WiFi network named "iPhone
Network". Next, assign a fixed IP address for the Airport interface,
such as "192.168.1.2".

On the iPhone, you need to set up the network to join the "iPhone
Network" WiFi network in "Settings->Wi-Fi". Since no DHCP server is
configured for this network, you also need to manually assign an IP
address to the WiFi interface; we'll use "192.168.1.3".

The OS X computer and the iPhone should now be able to
communicate. You can try pinging the iPhone from your OS X computer to
verify it.

On the iPhone, run the router application. This application listens
on port `12000` and on all network interfaces by default.

You are now ready to try the Cocoa library demo located in the
`objective-c/IceTouch/Cocoa`directory of this demo repository. To use the
iPhone router, you need to modify the applications `client.config` settings
file to use option 3 or 4 and set `<host>` to the iPhone's IP address
(192.168.1.3). The client will connect to the library server on the internet
through the iPhone's cellular connection.
