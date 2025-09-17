# Bidir

The Bidir demo illustrates how to send requests "the other way around", from a server to a client, by reusing the
connection established by the client to the server.

This demo is very similar to the [../Callback] demo: with the Callback demo, the server opens a connection to the
client, while with this demo, there is only one connection, from the client to the server:

```mermaid
flowchart LR
    c[Client<br>hosts AlarmClock] --bidir connection--> s[Server:4061<br>hosts WakeUpService]
```

This is particularly useful when the client application is behind a firewall that does not allow incoming connections.

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
