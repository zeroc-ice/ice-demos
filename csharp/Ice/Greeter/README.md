# Ice Greeter

This demo illustrates how to send a request and wait for the response.

It provides two implementations for the server: a synchronous dispatch implementation (in Server), and an asynchronous
dispatch implementation (in ServerAMD). The same client works with both.

You can build the client and server applications with:

``` shell
dotnet build
```

First start the Server program:

```shell
cd Server
dotnet run
```

or

```shell
cd ServerAMD
dotnet run
```

In a separate terminal, start the Client program:

```shell
cd Client
dotnet run
```
