# Ice Custom Error

The Custom error demo shows how to define an exception in Slice, and how to throw and catch this exception.

A Slice-defined exception should be seen as a custom error carried by the response instead of the expected return
value--there is naturally no throwing across the network.

To build the demo, run:

```shell
cmake -B build -S . -G Ninja
cmake --build build
```

To run the demo, first start the server:

**Linux/macOS:**

```shell
./build/server
```

**Windows:**

```shell
build\server
```

In a separate window, start the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\client
```
