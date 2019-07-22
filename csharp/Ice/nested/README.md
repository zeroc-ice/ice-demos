A demo to illustrate how [nested callbacks][1] work, and how the size of
the thread pool affects the maximum nesting depth.

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server`           | `dotnet server.dll`  |

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |

[1]: https://doc.zeroc.com/ice/3.7/client-server-features/the-ice-threading-model/nested-invocations
