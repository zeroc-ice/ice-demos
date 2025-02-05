# Optional

This demo illustrates the use of [optional class members][1] and
[parameters][2].

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first start the server:

**Linux/macOS:**

```shell
./build/server
```

**Windows:**

```shell
build\Release\server
```

In a separate window, start the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

[1]: https://doc.zeroc.com/ice/3.7/the-slice-language/optional-data-members
[2]: https://doc.zeroc.com/ice/3.7/the-slice-language/interfaces-operations-and-exceptions/operations
