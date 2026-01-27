This demo is the Ice hello world demo configured to use [IceDiscovery].

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

[IceDiscovery]: https://archive.zeroc.com/ice/3.7/ice-plugins/icediscovery
[timeouts]: https://archive.zeroc.com/ice/3.7/client-side-features/invocation-timeouts
