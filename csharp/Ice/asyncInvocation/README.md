This demo illustrates the use of [Asynchronous Method Invocation (AMI)][1]
with a server that performs simple calculations and a client that can
call for the calculations without blocking.

To run the demo, first start the server:
```
dotnet run --project server\server.csproj
```
In a separate window, start the client:
```
dotnet run --project client\client.csproj
```

[1]: https://doc.zeroc.com/ice/4.0/language-mappings/c-sharp-mapping/client-side-slice-to-c-sharp-mapping/asynchronous-method-invocation-ami-in-c-sharp
