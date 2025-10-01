# Ice Greeter

The Greeter demo illustrates how to send a request and wait for the response.

It provides two implementations for the server: a synchronous dispatch implementation (in Server), and an asynchronous
dispatch implementation (in ServerAMD). The same client works with both.

## Building the demo

To build the demo, run:

```shell
swift build
```

## Running the demo

First start the Server program:

```shell
swift run Server
```

or

```shell
swift run ServerAMD
```

In a separate terminal, start the Client program:

```shell
swift run Client
```
