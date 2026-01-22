# Ice latency

A simple latency test that measures the basic call dispatch delay of Ice.

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
