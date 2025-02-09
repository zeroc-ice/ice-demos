# Invoke

The Invoke shows how to implement a Greeter client and server using [Dynamic Ice][1]. This demo does not use any
Slice definition or generated code; the client and server are nevertheless compatible with the client and server from
the Greeter demo.

You can build the client and server applications with:

``` shell
dotnet build
```

First start the Server program:

```shell
cd Server
dotnet run
```

In a separate terminal, start the Client program:

```shell
cd Client
dotnet run
```

[1]: https://doc.zeroc.com/ice/3.7/client-server-features/dynamic-ice
