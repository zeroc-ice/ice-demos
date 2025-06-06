# Ice for C# Demos

This folder contains example Ice applications written in C#. Each example is a simple client-server application that
demonstrates a specific feature or programming technique.

## Building Requirements

1. **.NET SDK 8.0**
   Download and install the .NET 8.0 SDK from
   [dotnet.microsoft.com](https://dotnet.microsoft.com/en-us/download/dotnet).

2. **ZeroC NuGet Feed**
   The included [nuget.config](./nuget.config) file already adds the ZeroC NuGet feed to make
   the ZeroC nightly builds available.

## Building the Demos

The top-level [Directory.Build.props](./Directory.Build.props) file sets `IceVersion` to
`3.8.0-nightly.*` by default to pull the latest nightly build of Ice.
You can override this on the command line:

```shell
dotnet build -p:IceVersion="3.8.0-alpha0"
```

## C# Demos

|                                                         |                                                                                                           |
| ------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| [Glacier2 Callback](./Glacier2/Callback/)               | Shows how to write a Glacier2 client and implement callbacks in this client.                              |
| [Glacier2 Greeter](./Glacier2/Greeter/)                 | Shows how to write a basic Glacier2 client.                                                               |
| [Ice Bidir](./Ice/Bidir/)                               | Shows how to send requests "the other way around", from the server to the client.                         |
| [Ice Callback](./Ice/Callback/)                         | Shows how to implement callbacks in a client application.                                                 |
| [Ice Cancellation](./Ice/Cancellation/)                 | Shows how to cancel a request and how to configure invocation timeouts.                                   |
| [Ice Config](./Ice/Config/)                             | Shows how to configure client and server applications using Ice configuration files.                      |
| [Ice Context](./Ice/Context/)                           | Shows how to set and retrieve request contexts.                                                           |
| [Ice Custom Error](./Ice/CustomError/)                  | Shows how to define a new exception in Slice and return this exception from a Slice operation.            |
| [Ice Filesystem](./Ice/Filesystem/)                     | Shows the power of interface inheritance in Slice.                                                        |
| [Ice Forwarder](./Ice/Forwarder/)                       | Shows how to write a servant that forwards all requests it receives to another remote Ice object.         |
| [Ice Greeter](./Ice/Greeter/)                           | Shows how to call and implement a canonical Greeter application with Ice. **Start with this demo!**       |
| [Ice Middleware](./Ice/Middleware/)                     | Shows how to write a simple middleware and how to add this middleware to an object adapter.               |
| [Ice Optional](./Ice/Optional/)                         | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.     |
| [IceBox Greeter](./IceBox/Greeter/)                     | Shows how to create an IceBox service.                                                                    |
| [IceDiscovery Greeter](./IceDiscovery/Greeter/)         | Shows how to configure the IceDiscovery plug-in.                                                          |
| [IceDiscovery Replication](./IceDiscovery/Replication/) | Shows how to use the IceDiscovery plug-in with replicated servers.                                        |
| [IceGrid Greeter](./IceGrid/Greeter)                    | Shows how to create a simple IceGrid deployment.                                                          |
| [IceGrid IceBox](./IceGrid/IceBox/)                     | Shows how to deploy an IceBox server with IceGrid.                                                        |
| [IceGrid LocatorDiscovery](./IceGrid/LocatorDiscovery/) | Shows how to write a client application that configures its locator using the IceLocatorDiscovery plug-in.|
| [IceGrid Query](./IceGrid/Query/)                       | Shows how use the Query object provided by the IceGrid registry to lookup a well-known object by type.    |
| [IceStorm Weather](./IceStorm/Weather/)                 | Shows how to use IceStorm to create a simple pub-sub application.                                         |
