A demo to illustrate how [nested callbacks] work, and how the size of the thread pool affects the maximum nesting
depth.

To run the demo, first start the server:

```shell
dotnet server.dll
```

In a separate window, start the client:

```shell
dotnet client.dll
```

[nested callbacks]: https://archive.zeroc.com/ice/3.7/client-server-features/the-ice-threading-model/nested-invocations
