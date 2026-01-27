A simple callback demo that illustrates how a client can pass a proxy to a server, invoke an operation in the server,
and the [server calls back] into an object provided by the client as part of that invocation.

To run the demo, first start the server:

```shell
dotnet server.dll
```

In a separate window, start the client:

```shell
dotnet client.dll
```

[server calls back]: https://archive.zeroc.com/ice/3.7/client-server-features/the-ice-threading-model/nested-invocations
