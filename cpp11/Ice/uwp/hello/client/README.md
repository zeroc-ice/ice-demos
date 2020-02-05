# Hello Demo Client App (Universal Windows)

This demo is a graphical client app written in C++/CX. This client
works with the UWP hello server as well as the Ice/hello servers provided by
other language mappings, such as C++, C#, Java and Python.

If you want to run the server on the same computer as your UWP client,
you cannot use the UWP server (due to UWP restrictions). You can however
use any other hello server (C++, C# etc.).

## Running the demo

* Build (if needed) and start the Ice/hello server of your choice.
* Build and deploy the demo using `Deploy hello` from the `Build` menu.
* Start the "Ice Hello Demo Client" application from the Windows Start screen
or right click on the project and select `Debug -> Start new instance`.
* If you are connecting to a hello server running on the same machine
you must add the application to the loopback exempted list, using the
following command:
```
CheckNetIsolation LoopbackExempt -a -n=ice-hello-client_3qjctahehqazm
```
This is automatically done when you start the client application from
Visual Studio `Debug -> Start new instance`.

### SSL

If you run the hello client and server on different computers or devices, you
also need to regenerate the server certificate to ensure the certificate's
common name is set to the address used by the client to connect to the server.

To regenerate the certificate, you can run the `makedemocerts.py` Python script
from the `certs` directory at the top of this distribution. For example:

```bash
cd certs
makedemocerts.py --ip 192.168.1.53 --dns 192.168.1.53
```

This will regenerate a server certificate with a common name, IP address and
DNS name set to `192.168.1.53`. It's important to use the same value as the
value used in the client to connect to the server.

You can regenerate these certificates on the client or on the server. Then
make sure to copy these certificates (the `certs` directory) to the other
system.

 > Note: Rebuilding and redeploying the hello client app is not sufficient to
 > update its certificates. Make sure to manually un-install this app before
 > redeploying it.

### Timeouts

To test [timeouts][1] you can use the timeout slider to set a timeout in the
client proxy and the delay slider to set a delayed response in the server to
cause a timeout.

### IceDiscovery

Instead of configuring the IP address for the server, you can use
[IceDiscovery][2] to discover the server.

To enable IceDiscovery, click on the "Use IceDiscovery" check-box. You must run
the hello server from the IceDiscovery/hello demo and ensure that the server is
listening on an accessible network interface.

See the README.md file of the IceDiscovery hello demo for more information on
how to start the server.

> Note: you must run the server on another machine. The hello client can't
> discover servers running on the local machine due to network isolation
> restrictions of Windows Store applications.

[5]: https://doc.zeroc.com/ice/4.0/client-side-features/invocation-timeouts
[2]: https://doc.zeroc.com/ice/4.0/ice-plugins/icediscovery
