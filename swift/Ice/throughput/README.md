# Ice throughput

A simple throughput demo that allows you to send sequences of various types between client and server and to measure the
maximum bandwidth that can be achieved using serialized synchronous requests.

## Building the Demo

```shell
make slice
swift build
```

## Running the Demo

First start the server:

```shell
.build/debug/Server
```

In a separate window, start the client:

```shell
.build/debug/Client
```
