# Ice for C# Demos

This folder contains example Ice applications written in C#. Each example is a simple client-server application that
demonstrates a specific feature or programming technique.

The MSBuild projects download and install the Ice for C# NuGet packages as needed. See [Ice for C# installation] for
details.

|                                                         |                                                                                                             |
| ------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- |
| [Glacier2 Callback](./Glacier2/Callback/)               | Shows how to write a Glacier2 client and implement callbacks in this client.                                |
| [Glacier2 Greeter](./Glacier2/Greeter/)                 | Shows how to write a basic Glacier2 client.                                                                 |
| [Glacier2 Session](./Glacier2/Session)                  | Shows how to implement and configure a custom Glacier2 Session Manager. Also demonstrates default servants. |
| [Ice Bidir](./Ice/Bidir/)                               | Shows how to send requests "the other way around", from the server to the client.                           |
| [Ice Callback](./Ice/Callback/)                         | Shows how to implement callbacks in a client application.                                                   |
| [Ice Cancellation](./Ice/Cancellation/)                 | Shows how to cancel a request and how to configure invocation timeouts.                                     |
| [Ice Config](./Ice/Config/)                             | Shows how to configure client and server applications using Ice configuration files.                        |
| [Ice Context](./Ice/Context/)                           | Shows how to set and retrieve request contexts.                                                             |
| [Ice Custom Error](./Ice/CustomError/)                  | Shows how to define a new exception in Slice and return this exception from a Slice operation.              |
| [Ice Forwarder](./Ice/Forwarder/)                       | Shows how to write a servant that forwards all requests it receives to another remote Ice object.           |
| [Ice Greeter](./Ice/Greeter/)                           | Shows how to call and implement a canonical Greeter application with Ice. **Start with this demo!**         |
| [Ice Inheritance](./Ice/Inheritance/)                   | Shows the power of interface inheritance in Slice.                                                          |
| [Ice Middleware](./Ice/Middleware/)                     | Shows how to write a simple middleware and how to add this middleware to an object adapter.                 |
| [Ice Optional](./Ice/Optional/)                         | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.       |
| [Ice Secure](./Ice/secure/)                             | Shows how to use the Ice.SSL APIs to configure the ssl transport.                                           |
| [IceBox Greeter](./IceBox/Greeter/)                     | Shows how to create an IceBox service.                                                                      |
| [IceDiscovery Greeter](./IceDiscovery/Greeter/)         | Shows how to configure the IceDiscovery plug-in.                                                            |
| [IceDiscovery Replication](./IceDiscovery/Replication/) | Shows how to use the IceDiscovery plug-in with replicated servers.                                          |
| [IceGrid Greeter](./IceGrid/Greeter)                    | Shows how to create a simple IceGrid deployment.                                                            |
| [IceGrid IceBox](./IceGrid/IceBox/)                     | Shows how to deploy an IceBox server with IceGrid.                                                          |
| [IceGrid LocatorDiscovery](./IceGrid/LocatorDiscovery/) | Shows how to write a client application that configures its locator using the IceLocatorDiscovery plug-in.  |
| [IceGrid Query](./IceGrid/Query/)                       | Shows how use the Query object provided by the IceGrid registry to lookup a well-known object by type.      |
| [IceStorm Weather](./IceStorm/Weather/)                 | Shows how to use IceStorm to create a simple pub-sub application.                                           |

[Ice for C# installation]: https://zeroc.com/ice/downloads/3.8/cpp-1
