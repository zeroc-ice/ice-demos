# Ice InvocationTimeout

The InvocationTimeout demo shows how to configure invocation timeouts on proxies. It also shows how to cancel
dispatches.

You can build the client and server applications with:

```shell
swift build
```

First start the Server program:

```shell
swift run Server
```

In a separate terminal, start the Client program:

```shell
swift run Client --Ice.Trace.Network
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that
> `InvocationTimeoutException` does not close the connection.
