# Ice asyncInvocation

This demo illustrates the use of Asynchronous Method Invocation (AMI) with a server that performs simple calculations
and a client that can call for the calculations without blocking.

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

In a second window, start the client:

```shell
.build/debug/Client
```
