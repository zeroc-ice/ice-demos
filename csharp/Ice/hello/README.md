This demo illustrates how to invoke ordinary (twoway) operations, as well as how to make [oneway], [datagram], [secure],
and [batched] invocations.

To run the demo, first start the server:

```shell
dotnet server.dll
```

In a separate window, start the client:

```shell
dotnet client.dll
```

To test [timeouts] you can use 'T' to set an invocation timeout on the client proxy and 'P' to set a delayed response
in the server to cause a timeout.

[oneway]: https://archive.zeroc.com/ice/3.7/client-side-features/oneway-invocations
[datagram]: https://archive.zeroc.com/ice/3.7/client-side-features/datagram-invocations
[secure]: https://archive.zeroc.com/ice/3.7/ice-plugins/icessl
[batched]: https://archive.zeroc.com/ice/3.7/client-side-features/batched-invocations
[timeouts]: https://archive.zeroc.com/ice/3.7/client-side-features/invocation-timeouts
