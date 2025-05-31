# IceGrid LocatorDiscovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plugin.

This demo provides a client application that works with the IceGrid/greeter demo and reuses its server components and
IceGrid configuration.

Follow these steps to build and run the demo:

1. Build the [IceGrid/greeter](../greeter) demo.

2. Start the IceGrid registry in its own terminal:

   ```shell
   icegridregistry --Ice.Config=../greeter/config.registry
   ```

3. Start the IceGrid node in its own terminal:

   ```shell
   icegridnode --Ice.Config=../greeter/config.node
   ```

4. Deploy the "GreeterHall" application in this IceGrid deployment:

   ```shell
   icegridadmin --Ice.Config=../greeter/config.admin -e "application add ../greeter/greeter-hall.xml"
   ```

   > [!TIP]
   > Use `update` instead of `add` to update an existing application.

5. Build and run the client application:

   ```shell
   swift build
   swift run Client
   ```

> [!NOTE]
> The `Ice.Plugin.IceLocatorDiscovery=1` property in the client enables automatic discovery of the IceGrid registry's
> locator, eliminating the need to manually configure the default locator.

> [!IMPORTANT]
> This demo is only supported on macOS because the ice-swift dependency is only available for macOS.
