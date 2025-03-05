This demo shows how to use replication with [IceDiscovery][1].

To run the demo, start the 3 servers:
```
server --Ice.Config=config.server1
server --Ice.Config=config.server2
server --Ice.Config=config.server3
```

In a separate window, start the client:
```
client
```

> With .NET 6, use instead:
> ```
> dotnet server.dll --Ice.Config=config.server1
> dotnet server.dll --Ice.Config=config.server2
> dotnet server.dll --Ice.Config=config.server3
> ```
> and
> ```
> dotnet client.dll
> ```

The client invokes the number of specified iterations with a given
delay on a well-known proxy configured to use per-request load
balancing. Each invocation on the proxy queries the Ice locator
implemented by the IceDiscovery plug-in.

While the client is running and invoking on the server, you can try to
stop some of the servers. As long as one server is still running, the
client will continue to work.

[1]: https://doc.zeroc.com/ice/3.7/ice-plugins/icediscovery
