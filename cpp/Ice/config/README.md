# Config

This demo shows how to configure your client and server applications using Ice configuration files.

## Ice prerequisites

- Install the C++ dev kit.
  - Linux and macOS: see [Ice for C++ installation].
  - Windows: the cmake build downloads and installs the C++ dev kit automatically.

## Building the demo

To build the demo, run:

```shell
cmake -B build -S . -G Ninja
cmake --build build
```

## Running the demo

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

You can pass `--Ice` command-line options to set additional properties or override the properties set in the
configuration file.

For example:

**Linux/macOS:**

```shell
./build/server --Ice.Default.Protocol=ws
```

```shell
./build/client --Ice.Default.Protocol=ws --Ice.Trace.Network=2
```

**Windows:**

```shell
build\server --Ice.Default.Protocol=ws
```

```shell
build\client --Ice.Default.Protocol=ws --Ice.Trace.Network=2
```

[Ice for C++ installation]: https://zeroc.com/ice/downloads/3.8/cpp
