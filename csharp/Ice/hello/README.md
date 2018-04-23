This demo illustrates how to invoke ordinary (twoway) operations, as
well as how to make [oneway][1], [datagram][2], [secure][3], and
[batched][4] invocations.

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server`           | `dotnet server.dll`  |

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |

To test [timeouts][5] you can use 'T' to set an invocation timeout on the
client proxy and 'P' to set a delayed response in the server to cause a
timeout.

[1]: https://doc.zeroc.com/display/Ice37/Oneway+Invocations
[2]: https://doc.zeroc.com/display/Ice37/Datagram+Invocations
[3]: https://doc.zeroc.com/display/Ice37/IceSSL
[4]: https://doc.zeroc.com/display/Ice37/Batched+Invocations
[5]: https://doc.zeroc.com/display/Ice37/Invocation+Timeouts
