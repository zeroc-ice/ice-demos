This demo illustrates the use of [Asynchronous Method Invocation (AMI)][1]
with a server that performs simple calculations and a client that can
call for the calculations without blocking.

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server`           | `dotnet server.dll`  |

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |

[1]: https://doc.zeroc.com/display/Ice37/Asynchronous+Method+Invocation+%28AMI%29+in+C-Sharp
