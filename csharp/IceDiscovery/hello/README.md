This demo is the Ice hello world demo configured to use [IceDiscovery][1].

To run the demo, first start the server:
```
dotnet run --project server\server.csproj
```
In a separate window, start the client:
```
dotnet run --project client\client.csproj
```

To test [timeouts][2] you can use 'T' to set an invocation timeout on the
client proxy and 'P' to set a delayed response in the server to cause a
timeout.

[1]: https://doc.zeroc.com/ice/4.0/ice-plugins/icediscovery
[5]: https://doc.zeroc.com/ice/4.0/client-side-features/invocation-timeouts
