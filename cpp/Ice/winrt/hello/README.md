# Hello Demo Client App for WinRT and Universal Windows Platform (UWP)

This demo is a graphical client app written in C++/CX. This client
works with the Ice/hello server provided by other language mappings,
such as C++, C#, Java and Python.

 > Note: it is possible to implement a "hello server" in C++/CX
 > for WinRT / UWP as Ice for WinRT / UWP provide server side 
 > support. Such a "hello server" will be provided soon.

## Running the demo

* Build (if needed) and start the Ice/hello server of your choice.
* Build and deploy the demo using `Deploy hello` from the `Build` menu.
* Start the "Ice Hello Demo" application from the Windows Start screen
or right click on the project and select `Debug -> Start new instance`.

### SSL

The Ice ssl transport for WinRT / UWP currently does not support client-side 
authentication. In order to successfully connect with SSL, you need to edit 
the hello server configuration file (`config.server`) and uncomment the 
`IceSSL.VerifyPeer=0` property. 

If you run the hello client and server on different computers or devices,
you also need to regenerate the server certificate to ensure the 
certificate common name is set to the IP address of the server. To 
regenerate the certificate, you can run the `makecerts.py` Python script 
from the `certs` directory at the top of this distribution. For example:

```bash
cd certs
makecerts.py 192.168.1.53
```

This will regenerate a server certificate with a common name set to
192.168.1.53 (please use your server's IP address). You can provide an IP address
or DNS name, the only requirement is that it matches the value used by the
hello client to connect to the hello server.

You can regenerate these certificates on the client or on the server. Then
make sure to copy these certificates (the `certs` directory) to the other system. 

 > Note: Rebuilding and redeploying the hello client app is not sufficient to
 > update its certificates. Make sure to manually un-install this app before
 > redeploying it.

### Timeouts

To test timeouts you can use the timeout slider to set a timeout in
the client proxy and the delay slider to set a delayed response in
the server to cause a timeout.

You will notice that two "Hello World!" messages will be printed by
the server in this case. This is because the sayHello method is marked
as idempotent in the Slice, meaning that Ice does not need to follow
the at-most-once retry semantics.

See the manual for more information about retry behavior.

### IceDiscovery

Instead of configuring the IP address for the server you can use IceDiscovery to
discover the server.

To enable IceDiscovery, click on the "Use IceDiscovery" check-box. You must run
the hello server from the IceDiscovery/hello demo and ensure that the server is
listening on an accessible network interface.

See the README file of the IceDiscovery hello demo for more information on how
to start the server.

> Note: you must run the server on another machine. The hello client can't
> discover servers running on the local machine due to network isolation
> restrictions of Windows Store applications.
