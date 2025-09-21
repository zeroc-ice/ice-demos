# IceGrid LocatorDiscovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plug-in.

This demo provides a client application that works with the IceGrid/greeter demo and reuses its server components and
IceGrid configuration.

## Ice prerequisites

- Install the IceGrid registry and node. See [Ice service installation].

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

First, build the [IceGrid/Greeter](../Greeter) demo.

Then, run `icegridregistry`, `icegridnode`, and configure IceGrid using `icegridadmin` as per the IceGrid Greeter demo
instructions.

Finally, run the client application:

```shell
cd Client
dotnet run
```

[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
