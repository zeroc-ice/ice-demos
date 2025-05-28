# Context

The Context demo shows the 3 different ways to set a request context in a client.

A request context is a `dictionary<string, string>` carried by all requests. It is empty by default, and the
application is free to set any entry in this dictionary.

To build the demo, run:

```shell
cmake -B build
cmake --build build --config Release
```

## macOS

On macOS, the server certificate and private key must be imported into a keychain before the server application can access them.

We use the `security import` command to import the server’s PKCS#12 file (`.p12`) into the default keychain (typically the login keychain). The `-T` option is used to grant access to the server application binary:

```shell
security import ../../../certs/server.p12 -T build/server
```
