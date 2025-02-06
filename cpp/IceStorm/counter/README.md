# Counter

This demo shows how to implement the observer pattern using [IceStorm][1].

To build the demo run:

```shell
cmake -B build -S .
cmake --build build --config Release
```

To run the demo, start the IceStorm service:

```shell
icebox --Ice.Config=config.icebox
```

This configuration assumes there is a subdirectory named db in the
current working directory.

In a separate window, start the server:

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

Press 'i' and 'd' to increment and decrement the counter. You can also
start more clients in other windows and observe that each is
initialized and subsequently updated with the current counter value.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icestorm
