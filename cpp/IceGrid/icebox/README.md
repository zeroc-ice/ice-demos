# IceGrid IceBox

The IceGrid IceBox demo illustrates how to deploy an IceBox server with IceGrid.

Follow these steps to build and run the demo:

1. Build the client and server applications:

    ```shell
    cmake -B build -S . -G Ninja
    cmake --build build
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

5. Start the client in its own terminal:

   **Linux/macOS:**

   ```shell
   ./build/client
   ```

   **Windows:**

   ```shell
   build\client
   ```
