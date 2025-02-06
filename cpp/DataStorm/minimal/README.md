# DataStorm Minimal

This demo is the minimal DataStorm "hello world" application.

To build the demo run:

```shell
cmake -B build -S .
cmake --build build --config Release
```

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
