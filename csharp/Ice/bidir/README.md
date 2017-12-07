This demo shows how to use bidirectional connections for callbacks.
This is typically used if the server cannot open a connection to the
client to send callbacks, for example, because firewalls block
incoming connections to the client.

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server`           | `dotnet server.dll`  |

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |
