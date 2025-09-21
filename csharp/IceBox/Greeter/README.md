#  IceBox Greeter

This demo shows how to create an IceBox service in C#.

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

First, you need to install the `iceboxnet` tool. The following command performs a local installation:

```shell
dotnet tool install iceboxnet --prerelease --create-manifest-if-needed
```

Then, open a new terminal to run your iceboxnet server:

```shell
dotnet iceboxnet --IceBox.Service.Greeter="Service/bin/Debug/net8.0/GreeterService.dll:Service.GreeterService --Ice.Trace.Dispatch"
```

The command above configures iceboxnet to load the Greeter service from the GreeterService assembly.

Finally, start the Client program:

```shell
cd Client
dotnet run
```
