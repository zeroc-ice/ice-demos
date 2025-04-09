# IceDiscovery Greeter

This demo illustrates how to configure the IceDiscovery plugin. The IceDiscovery plugin allows a client application
to discover Ice objects without hardcoding any addressing information.

To build the demo, run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first start the server:

**Linux/macOS:**

```shell
./build/server
```

**Windows:**

```shell
build\Release\server
```

In a separate window, start the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```
