# IceGrid LocatorDiscovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plugin.

This demo provides a new client application for the IceGrid/Greeter demo, and relies on IceGrid/Greeter for everything
else.

Follow these steps to build and run the demo:

1. Build the [IceGrid/Greeter](../greeter) demo.

2. Run `icegridregistry`, `icegridnode`, and configure IceGrid using `icegridadmin` as per the IceGrid Greeter demo
instructions.

3. Build and run the client application in this directory:

   ```shell
   cmake -B build
   cmake --build build --config Release
   ```

    **Linux/macOS:**

    ```shell
    ./build/client
    ```

    **Windows:**

    ```shell
    build\Release\client
    ```
