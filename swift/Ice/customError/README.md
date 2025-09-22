# Ice Custom Error

The Custom error demo shows how to define an exception in Slice, and how to throw and catch this exception.

A Slice-defined exception should be seen as a custom error carried by the response instead of the expected return
value--there is naturally no throwing across the network.

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
