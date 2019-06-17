This demo is the Ice hello world demo configured to use [IceDiscovery][1].

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server`           | `dotnet server.dll`  |

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |

To test [timeouts][2] you can use 'T' to set an invocation timeout on the
client proxy and 'P' to set a delayed response in the server to cause a
timeout.

[1]: https://doc.zeroc.com/ice/3.7/ice-plugins/icediscovery
[5]: https://doc.zeroc.com/ice/3.7/client-side-features/invocation-timeouts
