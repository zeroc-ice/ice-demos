# Callback

A simple callback demo that illustrates how a client can pass a proxy
to a server, invoke an operation in the server, and the [server call
back][1] into an object provided by the client as part of that invocation.

To run the demo, first start the server:

To build the demo run:

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

[1]: https://doc.zeroc.com/ice/3.7/client-server-features/the-ice-threading-model/nested-invocations
