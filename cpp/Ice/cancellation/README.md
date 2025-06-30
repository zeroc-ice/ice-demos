# Ice Cancellation

The Cancellation demo shows how to cancel an invocation in C++. It also shows a related feature: invocation timeouts.

To build the demo, run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first start the server:

**Linux/macOS:**

```shell
./build/server
```

**Windows:**

```shell
build\Release\server
```

In a separate window, start the client:

**Linux/macOS:**

```shell
./build/client --Ice.Trace.Network
```

**Windows:**

```shell
build\Release\client --Ice.Trace.Network
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that the
> `InvocationCanceledException` and `InvocationTimeoutException` do not close the connection.
