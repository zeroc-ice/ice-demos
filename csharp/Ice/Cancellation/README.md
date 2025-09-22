# Ice Cancellation

The Cancellation demo shows how to cancel an invocation using the C# cancellation token. It also shows a related
feature: invocation timeouts.

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

First, start the Server program:

```shell
cd Server
dotnet run
```

Then, in a separate terminal, start the Client program:

```shell
cd Client
dotnet run --Ice.Trace.Network
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that the
> `InvocationCanceledException` and `InvocationTimeoutException` do not close the connection.
