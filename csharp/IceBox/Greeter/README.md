# IceBox Greeter

This demo shows how to create an IceBox service in C#.

## Ice prerequisites

- Install the `iceboxnet` tool.
  The following command performs a local installation:

  ```shell
  dotnet tool install iceboxnet --create-manifest-if-needed
  ```

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

First, start the iceboxnet server:

```shell
dotnet iceboxnet --IceBox.Service.Greeter="Service/bin/Debug/net8.0/GreeterService.dll:Service.GreeterService --Ice.Trace.Dispatch"
```

The command above configures iceboxnet to load the Greeter service from the GreeterService assembly.

In a separate terminal, start the Client program:

```shell
cd Client
dotnet run
```
