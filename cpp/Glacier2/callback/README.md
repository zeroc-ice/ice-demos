# Glacier2 Callback

This demo shows how to write a client that establishes a session with a Glacier2 router. It also shows how to implement
callbacks in this client.

This demo is similar to the [Ice callback][1] demo, except all communications go through the Glacier router.

The connection between the client and the Glacier2 router is a "bidir" connection, like in the [Ice bidir][2] demo:

```mermaid
flowchart LR
    c[Client<br>hosts AlarmClock] --bidir connection--> g[Glacier2 router:4063]
    g --connection1--> s[Server:4061<br>hosts WakeUpService] --connection2--> g
```

## Ice prerequisites

- Install the C++ dev kit.
  - Linux and macOS: see [Ice for C++ installation].
  - Windows: the cmake build downloads and installs the C++ dev kit automatically.
- Install Glacier2. See [Ice service installation].

## Building the demo

To build the demo, run:

```shell
cmake -B build -S . -G Ninja
cmake --build build
```

## Running the demo

1. Start the Server program in its own terminal:

   **Linux/macOS:**

   ```shell
   ./build/server
   ```

   **Windows:**

   ```shell
   build\server
   ```

2. Start the Glacier2 router in its own terminal:

   ```shell
   glacier2router --Ice.Config=glacier2.conf
   ```

   > [!TIP]
   > You can also start the Glacier2 router before the server. The order does not matter: the server is identical to the
   > server provided in the [Ice Callback][1] demo and does not depend on Glacier2.

3. Start the client in its own terminal:

   **Linux/macOS:**

   ```shell
   ./build/client
   ```

   **Windows:**

   ```shell
   build\client
   ```

[1]: ../../Ice/callback
[2]: ../../Ice/bidir
[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
