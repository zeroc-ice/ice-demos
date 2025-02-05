# Multicast Talk

The command-line program allows you to discover other Linux or Android
devices using [multicast][1] and then hold a peer-to-peer talk session.

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first start the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

On another device, start the command-line program or Android app.

[1]: https://doc.zeroc.com/ice/3.7/client-side-features/datagram-invocations
