# IceGrid LocatorDiscovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plug-in.

This demo provides a client application that works with the IceGrid/greeter demo and reuses its server components and
IceGrid configuration.

Follow these steps to build and run the demo:

1. Build the [IceGrid/Greeter](../Greeter) demo.

2. Run `icegridregistry`, `icegridnode`, and configure IceGrid using `icegridadmin` as per the IceGrid Greeter demo
instructions.

3. Build and run the client application:

   ```shell
   cd Client
   dotnet run
   ```
