# Ice Context

The Context demo shows the 3 different ways to set a request context in a client.

A request context is a `dictionary<string, string>` carried by all requests. It is empty by default, and the application
is free to set any entry in this dictionary.

> [!NOTE]
> The Ice protocol does not support response contexts, only request contexts.

## Building the demo

To build the demo, run:

```shell
swift build
```

## Running the demo

First, start the Server program:

```shell
swift run Server
```

Then, in a separate terminal, start the Client program:

```shell
swift run Client
```
