# IceDiscovery Greeter

This demo illustrates how to use the IceDiscovery plug-in with Ice for MATLAB.

## Ice prerequisites

- Install the MATLAB dev kit. See [Ice for MATLAB installation].

## Building and running the demo

Ice for MATLAB supports only client-side applications. As a result, you first need to start a server implemented in a
language with server-side support, such as Python, Java, or C#.

You can start either the IceDiscovery Greeter server, or two or more IceDiscovery Replication servers.

Then, in the MATLAB console:

- Go to the IceDiscovery/greeter directory

```shell
cd matlab/IceDiscovery/greeter
```

- Compile the Greeter.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Greeter.ice
```

- Run the client application

```shell
client
```

You can also turn on logging for locator resolution by running:

```shell
client --Ice.Trace.Locator --Ice.LogFile=client.log
```

This call creates a log file named `client.log` in your current directory.

[Ice for MATLAB installation]: https://zeroc.com/ice/downloads/3.8/matlab
