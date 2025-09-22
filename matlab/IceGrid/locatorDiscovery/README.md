# IceGrid Locator Discovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plug-in.

## Ice prerequisites

- Install the MATLAB dev kit. See [Ice for MATLAB installation].

## Building and running the demo

Ice for MATLAB supports only client-side applications. As a result, you first need to deploy the IceGrid
Greeter server implemented in a language with server-side support, such as Python, Java, or C#.

Then, in the MATLAB console:

- Go to the IceGrid/locatorDiscovery directory

```shell
cd matlab/IceGrid/locatorDiscovery
```

- Compile the Greeter.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Greeter.ice
```

- Run the client application

```shell
client
```

[Ice for MATLAB installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-matlab
