# Custom error

The Custom error example shows how to define an operation that returns a `Result<Success, Failure>`; Success is the
type returned upon success while Failure is the type returned upon failure.

In this example, Success holds a string while Failure holds a user-defined type (an enum).

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