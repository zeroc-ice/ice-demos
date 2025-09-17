# DataStorm Minimal

This demo is the minimal DataStorm "hello world" application.

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
