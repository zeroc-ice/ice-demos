This example demonstrates how to clean up per-client objects through
the use of sessions.

When the client starts, a session object is created through which all
per-client objects are created. The server makes sure the connection
from the client remains open and destroys the session when it detects
the closure of this connection.

Once the session is destroyed, whether by the client destroying the
session upon termination or through the server detecting the closure
of the connection, all per-client objects are cleaned up by the
session object.

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server`           | `dotnet server.dll`  |

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |
