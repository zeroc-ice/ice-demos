# Context

The Context demo shows the 3 different ways to set a request context in a client.

A request context is a `dictionary<string, string>` carried by all requests. It is empty by default, and the
application is free to set any entry in this dictionary.

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
