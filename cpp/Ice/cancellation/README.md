# Cancellation

The Cancellation demo shows how to cancel an invocation in C++. It also shows a related feature: invocation timeouts.

You can build the client and server applications with:

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
