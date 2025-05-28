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

On macOS we have to import the server certificate in a Keychain before it can be used by the server application,
the next commands can be used to create a keychain and add our certificate and key to the new keychain:

```shell
security create-keychain ~/server.keychain
security import ../../../certs/server.p12 -k ~/server.keychain -T build/server 
```