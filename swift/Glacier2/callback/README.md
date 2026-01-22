# Glacier2 callback

This example demonstrates the use of several Glacier2 features, including [callbacks] and [request contexts].

## Building the Demo

```shell
make slice
swift build
```

## Running the Demo

First start the server:

```shell
.build/debug/Server
```

In a separate window, start the Glacier2 router:

```shell
glacier2router --Ice.Config=config.glacier2
```

In a separate window, start the client:

```shell
.build/debug/Client
```

If you plan to run this demo using clients on different hosts than the router, you must first modify the configuration.
You need to change the `Glacier2.Client.Endpoints` property in `config.glacier2` and the `Ice.Default.Router` property
in `config.client`. In all cases you must set the host parameter of the `-h host` endpoint option to the actual address
of the machine on which glacier2router is running.

[callbacks]: https://doc.zeroc.com/ice/3.7/ice-services/glacier2/callbacks-through-glacier2
[request contexts]: https://doc.zeroc.com/ice/3.7/ice-services/glacier2/how-glacier2-uses-request-contexts
