# Ice hello

This demo illustrates how to invoke ordinary (twoway) operations, as well as how to make [oneway], [datagram],
[secure], and [batched] invocations.

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

In a separate window, start the client:

```shell
.build/debug/Client
```

To test [timeouts] you can use 'T' to set an invocation timeout on the client proxy and 'P' to set a delayed response
in the server to cause a timeout.

[oneway]: https://doc.zeroc.com/ice/3.7/client-side-features/oneway-invocations
[datagram]: https://doc.zeroc.com/ice/3.7/client-side-features/datagram-invocations
[secure]: https://doc.zeroc.com/ice/3.7/ice-plugins/icessl
[batched]: https://doc.zeroc.com/ice/3.7/client-side-features/batched-invocations
[timeouts]: https://doc.zeroc.com/ice/3.7/client-side-features/invocation-timeouts
