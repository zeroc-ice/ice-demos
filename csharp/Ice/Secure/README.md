# Ice Secure

This demo illustrates how to programmatically configure client and server applications to use SSL secure connections.

You can build the client and server applications with:

```shell
dotnet build
```

First start the Server program:

```shell
cd Server
dotnet run --Ice.Trace.Network
```

In a separate terminal, start the Client program:

```shell
cd Client
dotnet run --Ice.Trace.Network
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that the
> requests are sent using the `ssl` secure transport.
