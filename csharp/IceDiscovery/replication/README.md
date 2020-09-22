This demo shows how to use replication with [IceDiscovery][1].

To run the demo, start the 3 servers:
```
dotnet run --project server\server.csproj -- --Ice.Config=config.server1
dotnet run --project server\server.csproj -- --Ice.Config=config.server2
dotnet run --project server\server.csproj -- --Ice.Config=config.server3
```

In a separate window, start the client:
```
dotnet run --project client\client.csproj
```

The client invokes the number of specified iterations with a given
delay on a well-known proxy configured to use per-request load
balancing. Each invocation on the proxy queries the Ice locator
implemented by the IceDiscovery plug-in.

While the client is running and invoking on the server, you can try to
stop some of the servers. As long as one server is still running, the
client will continue to work.

[1]: https://doc.zeroc.com/ice/4.0/ice-plugins/icediscovery
