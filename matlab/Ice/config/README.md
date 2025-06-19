# Ice Config

This demo shows how to configure a client application using an Ice configuration file.

Ice for MATLAB supports only client-side applications. As a result, you first need to start a Config server implemented
in a language with server-side support, such as Python, Java, or C#.

Then, in the MATLAB console:

- Go to the Ice/config directory

```shell
cd matlab/Ice/config
```

- Compile the Greeter.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Greeter.ice
```

- Run the client application

```shell
client
```

> [!NOTE]
> The client writes log messages to file client.log.

You can pass `--Ice` arguments to set additional properties or override the properties set in the configuration file.

For example:

```shell
client --Ice.Trace.Network=2 --Ice.LogFile=verbose.log
type verbose.log
```
