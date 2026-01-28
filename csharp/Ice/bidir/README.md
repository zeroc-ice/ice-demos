This demo shows how to use [bidirectional connections] for callbacks. This is typically used if the server cannot open
a connection to the client to send callbacks, for example, because firewalls block incoming connections to the client.

To run the demo, first start the server:

```shell
dotnet server.dll
```

In a separate window, start the client:

```shell
dotnet client.dll
```

[bidirectional connections]: https://archive.zeroc.com/ice/3.7/client-server-features/connection-management/bidirectional-connections
