# DataStorm KeyFilter

This demo illustrates the use of **key filters** in DataStorm.
The reader application creates two readers:

- The first reader uses the custom `startsWith` key filter to read samples whose keys start with `"floor1/"`.
- The second reader uses the built-in `_regex` key filter to read samples whose keys match a given regular expression.

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

[Ice for C++ installation]: https://zeroc.com/ice/downloads/3.8/cpp
