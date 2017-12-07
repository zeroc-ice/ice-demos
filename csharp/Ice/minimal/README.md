This demo is a minimal Ice "hello world" application. Each time the
client is run a "sayHello" invocation is sent to the server.

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server`           | `dotnet server.dll`  |

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |

Note that this demo uses port 10000. If port 10000 is not available on your
machine, you need to edit both client and server to use a free port.

The demo also assumes the client and server are running on the same host.
To run the demo on separate hosts, edit the server to remove `-h localhost`
from the object adapter's endpoint, and edit the client to replace `localhost`
with the host name or IP address of the server.

To run the server with WebSocket support:

| .NET Framework 4.5                 | .NET Core 2.0                                 |
| ---------------------------------- | --------------------------------------------- |
| `server --Ice.Default.Protocol=ws` | `dotnet server.dll --Ice.Default.Protocol=ws` |

To run the client with WebSocket support:

| .NET Framework 4.5                 | .NET Core 2.0                                 |
| ---------------------------------- | --------------------------------------------- |
| `client --Ice.Default.Protocol=ws` | `dotnet client.dll --Ice.Default.Protocol=ws` |
