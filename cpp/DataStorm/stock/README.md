# DataStorm Stock

This demo illustrates the use of partial updates. The writer adds stocks to the
topic and sends partial updates to update the stock price or volume. The reader
prints out the stock information and partial updates.

The demo uses Slice to define the `Demo::Stock` class in the `Stock.ice` file.

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

To run the demo, start the writer:

**Linux/macOS:**

```shell
./build/writer
```

**Windows:**

```shell
build\writer
```

In a separate window, start the reader:

**Linux/macOS:**

```shell
./build/reader
```

**Windows:**

```shell
build\reader
```

You can start multiple writers and readers to publish or follow different
stocks.

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
