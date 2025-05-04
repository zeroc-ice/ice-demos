# Ice Custom Error

The Custom error demo shows how to define an exception in Slice, and how to throw and catch this exception.

Such a Slice-defined exception should be seen as custom error carried by the response instead of the expected return
value--there is naturally no throwing across address spaces.

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
