# Properties

This demo illustrates how to access a server's [Properties facet][1]
in order to retrieve and modify its configuration properties. This
demo also shows how the server can receive notifications whenever its
properties are changed.

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


[1]: https://doc.zeroc.com/ice/3.7/administration-and-diagnostics/administrative-facility/the-properties-facet
