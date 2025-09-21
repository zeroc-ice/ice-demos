# Ice Config

This demo shows how to configure client and server applications using Ice configuration files.

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

You can pass `--Ice` command-line options to set additional properties or override the properties set in the
configuration file. For example:

```shell
swift run Server --Ice.Default.Protocol=ws
```

```shell
swift run Client --Ice.Default.Protocol=ws --Ice.Trace.Network=2
```
