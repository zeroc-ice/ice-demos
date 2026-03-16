# DataStorm Minimal

This demo is the minimal DataStorm "hello world" application.

## Ice prerequisites

- Install the C++ dev kit.
  - Linux and macOS: see [Ice for C++ installation].
  - Windows: the cmake build downloads and installs the C++ dev kit automatically.

## Building the demo

Configure the build:

```shell
cmake -B build -S . -G Ninja
```

On Windows, to get stack traces at runtime, you can download and copy the Ice PDB files to the build output directory
with the following configure command instead:

```shell
cmake -B build -S . -G Ninja -DICE_COPY_PDB=ON
```

Build the demo:

```shell
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

[Ice for C++ installation]: https://zeroc.com/ice/downloads/3.8/cpp
