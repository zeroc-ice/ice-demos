# IceGrid LocatorDiscovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plug-in.

This demo provides a client application that works with the IceGrid/greeter demo and reuses its server components and
IceGrid configuration.

## Ice prerequisites

- Install the C++ dev kit.
  - Linux and macOS: see [Ice for C++ installation].
  - Windows: the cmake build downloads and installs the C++ dev kit automatically.
- Install IceGrid. See [Ice service installation].

## Building the demo

Build the [IceGrid/greeter](../greeter) demo.

Build the client application in this directory:

```shell
cmake -B build
cmake --build build --config Release
```

## Running the demo

1. Run `icegridregistry`, `icegridnode`, and configure IceGrid using `icegridadmin` as per the IceGrid greeter demo
   instructions.

2. Run the client application in this directory:

    **Linux/macOS:**

    ```shell
    ./build/client
    ```

    **Windows:**

    ```shell
    build\Release\client
    ```

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
