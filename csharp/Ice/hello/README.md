This demo illustrates how to invoke ordinary (twoway) operations, as
well as how to make oneway, datagram, secure, and batched invocations.

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server.exe`       | `dotnet server.dll`  |

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `client.exe`       | `dotnet client.dll`  |

To test timeouts you can use 'T' to set an invocation timeout on the client
proxy and 'P' to set a delayed response in the server to cause a timeout.
