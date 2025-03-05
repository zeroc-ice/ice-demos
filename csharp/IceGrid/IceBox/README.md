# IceGrid IceBox

The IceGrid IceBox demo illustrates how to deploy an IceBox server with IceGrid.

Follow these steps to build and run the demo:

1. Build the client and server applications:

```shell
dotnet build
```

2. Install the `iceboxnet` tool:

```shell
dotnet tool install iceboxnet --prerelease --create-manifest-if-needed
```

3. Start the IceGrid registry in its own terminal:

```shell
icegridregistry --Ice.Config=config.registry
```

4. Start the IceGrid node in its own terminal:

```shell
icegridnode --Ice.Config=config.node
```

5. Deploy the "Simpsons" application in this IceGrid deployment:

```shell
icegridadmin --Ice.Config=config.admin -e "application add simpsons.xml"
```

6. Run the client application:

```shell
cd Client
dotnet run
```
