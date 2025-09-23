# Ice Greeter

The Greeter demo illustrates how to build a simple application with Ice.

It features a client which demonstrates the 3 APIs you can use to make remote invocations using generated Ice proxies:

- synchronous (`greet`),
- asynchronous with futures (`greetAsync`); and
- asynchronous with callbacks (another `greetAsync`)

This demo also provides two implementations for the server: a synchronous dispatch implementation (`server`), and an
asynchronous dispatch implementation (`serveramd`). The client works with both.

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

The build produces 3 executables: client, server, and serveramd.

## Running the demo

To run the demo, first start the server:

**Linux/macOS:**

```shell
./build/server
```

or

```shell
./build/serveramd
```

**Windows:**

```shell
build\server
```

or

```shell
build\serveramd
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

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
