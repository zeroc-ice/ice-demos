# Multicast

This demo illustrates how to use [UDP multicast][1] to implement a
discovery service.

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first start multiple instances of the server:

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

On startup the client will send out a broadcast message containing a
callback proxy and waits for a response. Any servers that are up and
listening on the broadcast port will then contact the client using the
callback saying they are available. The client selects a server and
proceeds.

By default this demo uses IPv4. If you want to use IPv6 UDP multicast
instead, uncomment the alternative IPv6 configuration in `config.client`
and `config.server`.

[1]: https://doc.zeroc.com/ice/3.7/client-side-features/datagram-invocations
