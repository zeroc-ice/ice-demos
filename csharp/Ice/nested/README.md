A demo to illustrate how [nested callbacks][1] work, and how the size of
the thread pool affects the maximum nesting depth.

To run the demo, first start the server:
```
server
```
In a separate window, start the client:
```
client
```

> With .NET 6, use instead:
> ```
> dotnet server.dll
> ```
> and
> ```
> dotnet client.dll
> ```

[1]: https://archive.zeroc.com/ice/3.7/client-server-features/the-ice-threading-model/nested-invocations
