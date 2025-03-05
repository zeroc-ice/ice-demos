#  IceBox Greeter

This demo shows how to create an [IceBox][1] service in C#.

You can build the client application and IceBox service with:

``` shell
dotnet build
```

To run this demo, you first need to install the `iceboxnet` tool. The following command performs a local installation:

```shell
dotnet tool install iceboxnet --prerelease --create-manifest-if-needed
```

Then, open a new terminal to run your iceboxnet server:

```shell
dotnet iceboxnet --IceBox.Service.Greeter="Service/bin/Debug/net8.0/GreeterService.dll:Service.GreeterService --Ice.Trace.Dispatch"
```

The command above configures iceboxnet to load its Greeter service from the GreeterService assembly.

Finally, start the Client program:

```shell
cd Client
dotnet run
```

[1]: https://doc.zeroc.com/ice/3.7/icebox
