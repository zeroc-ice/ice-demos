# Ice Invocation Timeout

The Invocation Timeout demo shows how to use invocation timeouts with Ice for MATLAB.

Ice for MATLAB supports only client-side applications. As a result, you first need to start a server implemented
in a language with server-side support, such as Python, Java, or C#. The corresponding demo server is called either
Ice Invocation Timeout or Ice Cancellation.

Then, in the MATLAB console:

- Go to the Ice/invocationTimeout directory

```shell
cd matlab/Ice/invocationTimeout
```

- Compile the Greeter.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Greeter.ice
```

- Run the client application

```shell
client({'--Ice.Trace.Network', '--Ice.LogFile=client.log'})
```

> [!NOTE]
> The `--Ice.Trace.Network` argument turns on Network tracing. For this demo, it shows you that the
> `InvocationTimeoutException` does not close the connection.
