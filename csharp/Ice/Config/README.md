# Ice Config

This demo shows how to configure client and server applications using Ice configuration files.

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

First, start the Server program:

```shell
cd Server
dotnet run
```

In a separate terminal, start the Client program:

```shell
cd Client
dotnet run
```

You can pass `--Ice` command-line options to set additional properties or override the properties set in the
configuration file. For example:

```shell
cd Server
dotnet run --Ice.Default.Protocol=ws
```

```shell
cd Client
dotnet run --Ice.Default.Protocol=ws --Ice.Trace.Network=2
```
