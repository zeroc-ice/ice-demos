# Ice InvocationTimeout

The InvocationTimeout demo shows how to configure invocation timeouts on proxies. It also shows how to cancel
dispatches.

## Building the demo

To build the demo, run:

```shell
swift build
```

## Running the demo

First, start the Server program:

```shell
swift run Server
```

Then, in a separate terminal, start the Client program:

```shell
swift run Client --Ice.Trace.Network
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that
> `InvocationTimeoutException` does not close the connection.
