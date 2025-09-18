# Ice for Swift Demos

This folder contains example Ice applications written in Swift. Each example is a simple client-server application that
demonstrates a specific feature or programming technique.

> Ice for Swift is only supported on macOS and iOS platforms.

## Building the Demos

Follow the instructions provided in each demo’s README.md file.

## Swift Demos

|                                                         |                                                                                                            |
|---------------------------------------------------------|------------------------------------------------------------------------------------------------------------|
| [Glacier2 Callback](./Glacier2/callback/)               | Shows how to write a Glacier2 client and implement callbacks in this client.                               |
| [Glacier2 Greeter](./Glacier2/greeter/)                 | Shows how to write a basic Glacier2 client.                                                                |
| [Ice Bidir](./Ice/bidir/)                               | Shows how to send requests "the other way around", from the server to the client.                          |
| [Ice Callback](./Ice/callback/)                         | Shows how to implement callbacks in a client application.                                                  |
| [Ice Config](./Ice/config/)                             | Shows how to configure client and server applications using Ice configuration files.                       |
| [Ice Context](./Ice/context/)                           | Shows how to set and retrieve request contexts.                                                            |
| [Ice Custom Error](./Ice/customError/)                  | Shows how to define a new exception in Slice and return this exception from a Slice operation.             |
| [Ice Forwarder](./Ice/forwarder/)                       | Shows how to write a servant that forwards all requests it receives to another remote Ice object.          |
| [Ice Greeter](./Ice/greeter/)                           | Shows how to call and implement a canonical Greeter application with Ice. **Start with this demo!**        |
| [Ice GreeterApp](./Ice/GreeterApp/)                     | Shows how to implement an Ice for Swift iOS application that uses SwiftUI                                  |
| [Ice Inheritance](./Ice/inheritance/)                   | Shows the power of interface inheritance in Slice.                                                         |
| [Ice Invocation Timeout](./Ice/invocationTimeout/)      | Shows how to use invocation timeouts.                                                                      |
| [Ice Middleware](./Ice/niddleware/)                     | Shows how to write a simple middleware and how to add this middleware to an object adapter.                |
| [Ice Optional](./Ice/optional/)                         | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.      |
| [IceDiscovery Greeter](./IceDiscovery/greeter/)         | Shows how to configure the IceDiscovery plug-in.                                                           |
| [IceDiscovery Replication](./IceDiscovery/replication/) | Shows how to use the IceDiscovery plug-in with replicated servers.                                         |
| [IceGrid Greeter](./IceGrid/greeter)                    | Shows how to create a simple IceGrid deployment.                                                           |
| [IceGrid LocatorDiscovery](./IceGrid/locatorDiscovery/) | Shows how to write a client application that configures its locator using the IceLocatorDiscovery plug-in. |
| [IceGrid Query](./IceGrid/query/)                       | Shows how use the Query object provided by the IceGrid registry to lookup a well-known object by type.     |
| [IceStorm Weather](./IceStorm/weather/)                 | Shows how to use IceStorm to create a simple pub-sub application.                                          |
