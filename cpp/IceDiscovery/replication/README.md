# IceDiscovery Replication

This demo shows how to use replication with [IceDiscovery][1].

To run the demo, start the 3 servers:

**Linux/macOS:**

```shell
./build/client --Ice.Config=config.server1
./build/client --Ice.Config=config.server2
./build/client --Ice.Config=config.server3
```

**Windows:**

```shell
build\Release\client --Ice.Config=config.server1
build\Release\client --Ice.Config=config.server2
build\Release\client --Ice.Config=config.server3
```

Server is located in the `build` directory on Linux/macOS and in the `build\Release` directory on Windows.

In a separate window:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

The client invokes the number of specified iterations with a given
delay on a well-known proxy configured to use per-request load
balancing. Each invocation on the proxy queries the Ice locator
implemented by the IceDiscovery plug-in.

While the client is running and invoking on the server, you can try to
stop some of the servers. As long as one server is still running, the
client will continue to work.

[1]: https://doc.zeroc.com/ice/3.7/ice-plugins/icediscovery
