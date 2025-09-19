# IceDiscovery Greeter

This demo illustrates how to use the IceDiscovery plug-in. The IceDiscovery plug-in allows a client application
to discover Ice objects without hardcoding any addressing information.

To build the demo, run:

```shell
cmake -B build -S . -G Ninja
cmake --build build
```

To run the demo, first start the server:

**Linux/macOS:**

```shell
./build/server
```

**Windows:**

```shell
build\server
```

In a separate window, start the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\client
```
