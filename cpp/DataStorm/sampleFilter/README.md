# DataStorm sampleFilter

This demo illustrates the use of sample filters. The reader uses the `_regex` predefined sample filter. The reader will
only receive samples matching the regular expression provided on construction of the reader object. Note that the
filtering of the samples is performed on the writer to minimize the number of events sent over the wire.

## Ice prerequisites

- Install the C++ dev kit.
  - Linux and macOS: see [Ice for C++ installation].
  - Windows: the cmake build downloads and installs the C++ dev kit automatically.

## Building the demo

To build the demo, run:

```shell
cmake -B build -S .
cmake --build build --config Release
```

## Running the demo

To run the demo, start the writer:

**Linux/macOS:**

```shell
./build/writer
```

**Windows:**

```shell
build\Release\writer
```

In a separate window, start the reader:

**Linux/macOS:**

```shell
./build/reader
```

**Windows:**

```shell
build\Release\reader
```

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
