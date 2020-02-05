# Bidir Demo Client App (Universal Windows)

This demo is a graphical client app written in C++/CX. This client works
with the UWP bidir server as well as the Ice/bidir servers provided by
other language mappings, such as C++, C#, Java and Python.

If you want to run the server on the same computer as your UWP client,
you cannot use the UWP server (due to UWP restrictions). You can however
use any other bidir server (C++, C# etc.).

This demo shows how to use [bidirectional connections][1] for callbacks.
This is typically used if the server cannot open a connection to the
client to send callbacks, for example, because firewalls block
incoming connections to the client.

## Running the demo

* Build (if needed) and start the Ice/bidir server of your choice.
* Build and deploy the demo using `Deploy client` from the `Build` menu.
* Start the "Ice Bidir Demo Client" application from the Windows Start screen
or right click on the project and select `Debug -> Start new instance`.
* If you are connecting to a bidir server running on the same machine
you must add the application to the loopback exempted list, using the
following command:
```
CheckNetIsolation LoopbackExempt -a -n=ice-bidir-client_3qjctahehqazm
```
This is automatically done when you start the client application from
Visual Studio `Debug -> Start new instance`.

[1]: https://doc.zeroc.com/ice/4.0/client-server-features/connection-management/bidirectional-connections
