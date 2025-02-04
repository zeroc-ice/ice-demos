# Greeter

The Greeter demo illustrates how to send a request and wait for the response.

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
.\build\Release\server
```

In a separate window, start the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
./build/Release/client
