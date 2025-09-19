# DataStorm sampleFilter

This demo illustrates the use of sample filters. The reader uses the `_regex` predefined sample filter. The reader will
only receive samples matching the regular expression provided on construction of the reader object. Note that the
filtering of the samples is performed on the writer to minimize the number of events sent over the wire.

To build the demo run:

```shell
cmake -B build -S . -G Ninja
cmake --build build
```

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
