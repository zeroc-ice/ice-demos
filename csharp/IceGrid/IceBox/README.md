# IceGrid IceBox

The IceGrid IceBox demo illustrates how to deploy an IceBox server with IceGrid.

## Ice prerequisites

- Install the IceGrid registry and node. See [Ice service installation].

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

First, install the `iceboxnet` tool:

```shell
dotnet tool install iceboxnet --prerelease --create-manifest-if-needed
```

Then, start the IceGrid registry in its own terminal:

```shell
icegridregistry --Ice.Config=config.registry
```

Next, start the IceGrid node in its own terminal:

```shell
icegridnode --Ice.Config=config.node
```

Deploy the "GreeterHall" application in this IceGrid deployment:

```shell
icegridadmin --Ice.Config=config.admin -e "application add greeter-hall.xml"
```

Finally, run the client application:

```shell
cd Client
dotnet run
```

[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
