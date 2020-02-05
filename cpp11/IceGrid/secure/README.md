This demo shows how to prevent unauthorized access to the [IceGrid registry
and node][1], to the [Glacier2 administrative router][2], and to the
[Ice.Admin][3] functionality of IceGrid-managed servers. These components
use IceSSL secure connections to communicate and restrict access to their
endpoints.

The client and server use a regular TCP endpoint to communicate, but
they could also use a secure endpoint if necessary.

To run the demo, you first need to generate certificates for the
IceGrid registry and node, the Glacier2 administrative router, and the
server. Run the `makedemocerts.py` python script in this directory to
create these certificates:

```
makedemocerts.py
```

Follow the instructions provided by the script. Note that to run the
script you must install the zeroc-icecertutils package from the
[Python package repository](https://pypi.python.org/pypi). To install
this package with [pip](https://pip.pypa.io):

```
pip install zeroc-icecertutils
```

The keytool utility from the Java Development Kit is also required to
generate Java KeyStore files so you need to add the the JDK bin
directory to your PATH (if it's not found, `makedemocerts.py` will print
a warning).

For simplicity, the certificates created by `makedemocerts.py` are not
protected with a password. In a real world deployment, to ensure that
only privileged users can create new certificates and start the
IceGrid components, you would typically use a password for the
certificate authority, the IceGrid registry and node certificates and
the Glacier2 certificate.

You could also protect the server certificate with a password and
specify the password in the server configuration in clear text.
However, this would not improve security as you would still rely on
filesystem permissions to restrict access to the configuration file,
so you might as well use a certificate without a password and rely on
the filesystem permissions to restrict access to the certificate.

Once the certificates are generated, you can start the IceGrid
registries, node, and Glacier2 router:

```
icegridregistry --Ice.Config=config.master
icegridregistry --Ice.Config=config.slave
icegridnode --Ice.Config=config.node
glacier2router --Ice.Config=config.glacier2
```

In a separate window:

```
icegridadmin --Ice.Config=config.admin -e "application add application.xml"
client
```

This will deploy the application described in the file
`application.xml` and start the client.

To use icegridadmin through the Glacier2 router, you can use the
following command:

```
icegridadmin --Ice.Config=config.admin --Ice.Default.Router="DemoGlacier2/router:ssl -p 4064"
```

Alternatively, you can edit the `config.admin` file and uncomment the
`Ice.Default.Router` property definition.

You can also manage this IceGrid deployment with the IceGridGUI.

The IceSSL configuration in `config.master`, `config.slave` and
`config.glacier2` is:

```
IceSSL.VerifyPeer=1
```

which means that a client, such as IceGridGUI, does not need to
provide an X.509 certificate. As a result, connecting with IceGrid
Admin is straightforward: you just need to create an SSL connection
with no associated X.509 certificate.

When using IceGridGUI, we recommend importing `certs/ca.jks` into the
IceGridGUI trusted CAs Key Store using the IceGridGUI
certificate manager. This will ensure IceGridGUI connects to the
correct IceGrid Registry or Glacier2 Router.

The icegridadmin command line tool configuration in `config.admin` sets
the property `IceSSL.CAs=ca.pem` to ensure that the admin
client is connecting to the correct registry or glacier2router. In a
Java client `IceSSL.Truststore=ca.jks` can be used for the same
purpose.

The IceGrid registry and Glacier2 router are configured to use the
"null permissions verifier" so you can use any username/password to
login with the admin tools. In a real world deployment, you would
instead typically use the file-based permissions verifier or a custom
permissions verifier implementation.

[1]: https://doc.zeroc.com/ice/4.0/ice-services/icegrid/securing-icegrid
[2]: https://doc.zeroc.com/ice/4.0/ice-services/glacier2/securing-a-glacier2-router
[3]: https://doc.zeroc.com/ice/4.0/administration-and-diagnostics/administrative-facility/security-considerations-for-administrative-facets
