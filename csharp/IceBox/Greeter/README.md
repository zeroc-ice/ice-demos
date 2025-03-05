#  IceBox Greeter

This demo shows how to create an [IceBox][1] service in C#.

You can build the client application and IceBox service with:

``` shell
dotnet build
```

To run this demo, you first need to install the `iceboxnet` tool:

```shell
dotnet tool install iceboxnet --global --prerelease
```

Then, open a new terminal to run your iceboxnet server:

```shell
iceboxnet --IceBox.Service.Greeter="Service/bin/Debug/net8.0/GreeterService.dll:Service.GreeterService --Ice.Trace.Dispatch"
```

The command above configures iceboxnet to load the Greeter service from the GreeterService assembly.

Finally, start the Client program:

```shell
cd Client
dotnet run
```
