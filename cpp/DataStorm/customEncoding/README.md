# DataStorm Custom Encoding

This demo illustrates how to implement a custom encoder and decoder for a topic value of type
`std::chrono::system_clock::time_point`.

The TimePoint.h header, included by both the reader and writer applications, defines the `DataStorm::Encoder` and
`DataStorm::Decoder` specializations required to encode and decode time_point values.

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

To run the demo, start the writer and specify the name of a city:

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

The reader will print the time sent by the writer. You can start multiple writers and readers.

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
