# Hello Demo Server App for WinRT and Universal Windows Platform (UWP)

This demo is a graphical server app written in C++/CX. This server
works with the Ice/hello client provided by other language mappings,
such as C++, C#, Java and Python.

## Running the demo

* Build and deploy the demo using `Deploy server` from the `Build` menu.
* Start the "Ice Hello Demo Server" application from the Windows Start screen
or right click on the project and select `Debug -> Start new instance`.
* On another machine/device build (if needed) and start the Ice/hello client
of your choice and connect using the public IP address of the machine running
the server.

### SSL

Since you need to run the hello client and server on different computers
or devices, you also need to regenerate the server certificate to ensure
the certificate common name is set to the IP address of the server. To 
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

 > Note: Rebuilding and redeploying the hello server app is not sufficient to
 > update its certificates. Make sure to manually un-install this app before
 > redeploying it.
