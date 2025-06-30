# DataStorm Stock

This demo illustrates the use of partial updates. The writer adds stocks to the
topic and sends partial updates to update the stock price or volume. The reader
prints out the stock information and partial updates.

The demo uses Slice to define the `Demo::Stock` class in the `Stock.ice` file.

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

You can start multiple writers and readers to publish or follow different
stocks.
