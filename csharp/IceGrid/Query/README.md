# IceGrid/Query

The IceGrid/Query demo shows how to use the Query object provided by the IceGrid registry to look-up a well-known
object by type.

Follow these steps to build and run the demo:

1. Build the client and server applications:

```shell
dotnet build
```

2. Start the IceGrid registry in its own terminal:

```shell
icegridregistry --Ice.Config=config.registry
```

3. Start the IceGrid node in its own terminal:

```shell
icegridnode --Ice.Config=config.node
```

4. Deploy the "GreeterHall" application in this IceGrid deployment:

```shell
icegridadmin --Ice.Config=config.admin -e "application add greeter-hall.xml"
```

5. Run the client application:

```shell
cd Client
dotnet run
```
