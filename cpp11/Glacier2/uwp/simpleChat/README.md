# SimpleChat Demo Client App (Universal Windows)

This demo is a graphical client app written in C++/CX. This client
works with the Glacier2/simpleChat server provided in C++, and demonstrates
the use of [Glacier2 session helpers][1].

## Running the demo

* Build and start the Glacier2/simpleChat server.
* Build and deploy this demo app using `Deploy chat` from the `Build` menu.
* Start the "Glacier2 Simple Chat Demo" application from the Windows Start screen
or right click on the project and select `Debug -> Start new instance`.

### SSL

The Ice SSL transport for UWP currently does not support client-side
authentication. In order to successfully connect with SSL, you need to edit
the Glacier2 router configuration file (`config.glacier2`) and uncomment the
`IceSSL.VerifyPeer=0` property.

If you run the chat client and Glacier2 router on different computers or
devices, you also need to regenerate the server certificate to ensure the
certificate's common name is set to the address used by the client to connect
to the Glacier2 router.

To regenerate the certificate, you can run the `makedemocerts.py` Python script
from the `certs` directory at the top of this distribution. For example:

```bash
cd certs
makedemocerts.py --ip 192.168.1.53 --dns 192.168.1.53
```

This will regenerate a server certificate with a common name, IP address and DNS
name set to `192.168.1.53`. It's important to use the same value as the value
used in the client to connect to the Glacier2 router.

You can regenerate these certificates on the client or on the server. Then make
sure to copy these certificates (the `certs` directory) to the other system.

 > Note: Rebuilding and redeploying the chat client app is not sufficient to
 > update its certificates. Make sure to manually un-install this app before
 > redeploying it.

[1]: https://doc.zeroc.com/ice/4.0/ice-services/glacier2/glacier2-sessionhelper-class
