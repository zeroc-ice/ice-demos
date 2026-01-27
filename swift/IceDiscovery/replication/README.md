# IceDiscovery replication

This demo shows how to use replication with [IceDiscovery].

## Building the Demo

```shell
make slice
swift build
```

## Running the Demo

Start the 3 servers:

```shell
.build/debug/Server --Ice.Config=config.server1
.build/debug/Server --Ice.Config=config.server2
.build/debug/Server --Ice.Config=config.server3
```

In a separate window:

```shell
.build/debug/Client
```

The client invokes the number of specified iterations with a given delay on a well-known proxy configured to use
per-request load balancing. Each invocation on the proxy queries the Ice locator implemented by the IceDiscovery
plug-in.

While the client is running and invoking on the server, you can try to stop some of the servers. As long as one server
is still running, the client will continue to work.

[IceDiscovery]: https://archive.zeroc.com/ice/3.7/ice-plugins/icediscovery
