# SimpleChat Demo Client App (Universal Windows)

This demo is a graphical client app written in C++/CX. This client
works with the Glacier2/simpleChat server provided in C++, and demonstrates 
the use of Glacier2 session helpers.

## Running the demo

* Build and start the Glacier2/simpleChat server.
* Build and deploy this demo app using `Deploy chat` from the `Build` menu.
* Start the "Glacier2 Simple Chat Demo" application from the Windows Start screen
or right click on the project and select `Debug -> Start new instance`.

### SSL

The Ice ssl transport for UWP currently does not support client-side 
authentication. In order to successfully connect with SSL, you need to edit 
the Glacier2 router configuration file (`config.router`) and uncomment the 
`IceSSL.VerifyPeer=0` property. 

If you run the chat client and Glacier2 router on different computers or 
devices, you also need to regenerate the server certificate to ensure the 
certificate common name is set to the IP address of the Glacier2 router. 
To regenerate the certificate, you can run the `makecerts.py` Python script 
from the `certs` directory at the top of this distribution. For example:

```bash
cd certs
makecerts.py --ip 192.168.1.53 --dns foo.bar.com
```

This will regenerate a server certificate with a common name and IP address set
to `192.168.1.53` and the DNS name set to `bar.foo.com`. To use the DNS name for
the common name, you can run the script with the `--use-dns` argument. You can
use either the IP address or DNS name for the common name, the only requirement
is that it matches the value used by the chat client to connect to the Glacier2
router.

You can regenerate these certificates on the client or on the server. Then
make sure to copy these certificates (the `certs` directory) to the other system. 

 > Note: Rebuilding and redeploying the chat client app is not sufficient to
 > update its certificates. Make sure to manually un-install this app before
 > redeploying it.
