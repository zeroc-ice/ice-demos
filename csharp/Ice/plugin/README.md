This demo illustrates how to write and configure a simple Ice plugin
as well as a Ice Logger plugin.

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server`           | `dotnet server.dll`  |

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |

Both the client and server use a custom logger which is loaded as
a logger plugin. The server also uses a plugin to implement the
servant for this demo.

Please Look in the client and sever configuration files to see the
plugin configuration.
