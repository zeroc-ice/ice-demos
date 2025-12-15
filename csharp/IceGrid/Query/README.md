# IceGrid Query

This demo shows how to use the Query object provided by the IceGrid registry to lookup a well-known object by type.

## Ice prerequisites

- Install IceGrid. See [IceGrid installation].

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

First, start the IceGrid registry in its own terminal:

```shell
icegridregistry --Ice.Config=registry.conf
```

Then, start the IceGrid node in its own terminal:

```shell
icegridnode --Ice.Config=node.conf
```

Next, deploy the "GreeterHall" application in this IceGrid deployment:

```shell
icegridadmin --Ice.Config=admin.conf -e "application add greeter-hall.xml"
```

Finally, run the client application:

```shell
cd Client
dotnet run
```

[IceGrid installation]: https://zeroc.com/ice/downloads/3.8/icegrid
