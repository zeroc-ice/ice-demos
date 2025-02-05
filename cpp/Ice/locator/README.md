# Locator

This demo illustrates how to write a simple [Locator][1].

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first start the locator:

**Linux/macOS:**

```shell
./build/locator
```

**Windows:**

```shell
.\build\Release\locator
```

In a separate window, first start the server:

**Linux/macOS:**

```shell
./build/server
```

**Windows:**

```shell
.\build\Release\server
```

In a separate window, start the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
.\build\Release\client
```

[1]: https://doc.zeroc.com/ice/3.7/client-server-features/locators
