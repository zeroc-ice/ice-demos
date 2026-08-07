# IceGrid IceBox

The IceGrid IceBox demo illustrates how to deploy an IceBox server with IceGrid.

## Ice prerequisites

- Install IceGrid. See [IceGrid installation].
- Install the `iceboxnet` tool.
  The following command performs a local installation:

  ```shell
  dotnet tool install iceboxnet --create-manifest-if-needed
  ```

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

Start the IceGrid registry in its own terminal, from this demo's directory:

```shell
icegridregistry --Ice.Config=registry.conf
```

Next, start the IceGrid node in its own terminal, from this demo's directory:

```shell
icegridnode --Ice.Config=node.conf
```

From this demo's directory, deploy the "GreeterHall" application in this IceGrid deployment:

```shell
icegridadmin --Ice.Config=admin.conf -e "application add greeter-hall.xml"
```

`greeter-hall.xml` configures the IceBox server to load the Greeter service from
`Service/bin/Debug/net8.0/GreeterService.dll`, the assembly produced by `dotnet build` with the default Debug
configuration; if you build the demo with `dotnet build -c Release`, update the `entry` path in this file.

Finally, run the client application:

```shell
cd Client
dotnet run
```

[IceGrid installation]: https://zeroc.com/ice/downloads/3.8/icegrid
