# Custom error

The Custom error example shows how to define an operation that throws an exception defined as
`GreeterException` when an issue is present.

In this example, `GreeterException` holds `reason` and `error`, values defined as a string and a
`GreeterError` meant to hold an error message and an error ID.

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