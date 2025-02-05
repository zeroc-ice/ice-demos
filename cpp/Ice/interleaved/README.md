The interleaved demo is a variant of the throughput demo that uses asynchronous
requests to send "echo" requests to the server. This way, the client sends
multiple concurrent requests and we maximize the bandwidth usage.

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

See the throughput demo for a description of the metadata used in
Throughput.ice.
