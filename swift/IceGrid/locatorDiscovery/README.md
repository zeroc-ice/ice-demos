# IceGrid LocatorDiscovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plug-in.

This demo provides a client application that works with the IceGrid/greeter demo and reuses its server components and
IceGrid configuration.

## Ice prerequisites

- Install IceGrid. See [Ice service installation].

## Building the demo

To build the demo, run:

```shell
swift build
```

## Running the demo

First, build the [IceGrid/greeter](../greeter) demo.

Then, run `icegridregistry`, `icegridnode`, and configure IceGrid using `icegridadmin` as per the IceGrid greeter demo
instructions.

Finally, run the client application:

```shell
swift run Client
```

[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services

> [!NOTE]
> The `Ice.Plugin.IceLocatorDiscovery=1` property in the client enables automatic discovery of the IceGrid registry's
> locator, eliminating the need to manually configure the default locator.
