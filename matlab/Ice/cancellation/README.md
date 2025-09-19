# Ice Cancellation

The Cancellation demo shows how to cancel an asynchronous invocation. It also shows a related feature: invocation
timeouts.

## Ice prerequisites

- Install the MATLAB dev kit. See [Ice for MATLAB installation].

## Building and running the demo

Ice for MATLAB supports only client-side applications. As a result, you first need to start a server implemented
in a language with server-side support, such as Python, Java, or C#.

Then, in the MATLAB console:

- Go to the Ice/cancellation directory

```shell
cd matlab/Ice/cancellation
```

- Compile the Greeter.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Greeter.ice
```

- Run the client application

```shell
client --Ice.Trace.Network --Ice.LogFile=client.log
```

> [!NOTE]
> The `--Ice.Trace.Network` argument turns on Network tracing. For this demo, it shows you that
> `InvocationTimeoutException` and `InvocationCanceledException` do not close the connection.

[Ice for MATLAB installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-matlab
