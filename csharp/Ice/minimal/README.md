This demo is a minimal Ice "hello world" application. Each time the client is
run a "sayHello" invocation is sent to the server.

To run the demo, first start the server:
```
dotnet run --project server\server.csproj
```
In a separate window, start the client:
```
dotnet run --project client\client.csproj
```

Note that this demo uses port 10000. If port 10000 is not available on your
machine, you need to edit both client and server to use a free port.

The demo also assumes the client and server are running on the same host.
To run the demo on separate hosts, edit the server to remove `-h localhost`
from the object adapter's endpoint, and edit the client to replace `localhost`
with the host name or IP address of the server.

To run the client and server with WebSocket support:
```
server --Ice.Default.Protocol=ws
```
and
```
client --Ice.Default.Protocol=ws
```

> With .NET Core 2.x, use instead:
> ```
> dotnet server.dll --Ice.Default.Protocol=ws
> ```
> and
> ```
> dotnet client.dll --Ice.Default.Protocol=ws
> ```
