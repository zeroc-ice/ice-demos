# IceGrid Hello

This demo is the Ice hello world demo configured to use [IceDiscovery][1].

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first start the server:

**Linux/macOS:**

```shell
./build/server
```

**Windows:**

```shell
build\Release\server
```

In a separate window, start the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

The client and server use IceDiscovery to enable the location of the
Ice hello object. See the config.server and config.client files for
details on the IceDiscovery configuration.

[1]: https://doc.zeroc.com/ice/3.7/ice-plugins/icediscovery
