# Ice for PHP Demos

This directory contains example Ice applications written in PHP. Each example is a simple client application
that demonstrates a specific feature or programming technique. The corresponding server is provided by the C++, C#,
Java, and Python demos.

> Ice for PHP is only supported on Linux and macOS platforms.

## Linux Prerequisites

- A supported Linux Distribution (RHEL 9/10, Amazon Linux 2023, Debian 12, Ubuntu 24.04)
- [Nightly build of Ice](https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#linux)

## macOS Prerequisites

- macOS Sequoia
- Ice for PHP source build, see [building Ice for PHP](https://github.com/zeroc-ice/ice/blob/main/php/BUILDING.md)

## Building the demos

Follow the instructions provided in each demo’s README.md file.

## PHP Demos

|                                                          |                                                                                                            |
|----------------------------------------------------------|------------------------------------------------------------------------------------------------------------|
| [Glacier2 Greeter](./Glacier2/greeter/)                  | Shows how to write a basic Glacier2 client.                                                                |
| [Ice Config](./Ice/config/)                              | Shows how to configure a client application using an Ice configuration file.                               |
| [Ice Context](./Ice/context/)                            | Shows different ways to set request contexts.                                                              |
| [Ice Custom Error](./Ice/customError/)                   | Shows how to define a new exception in Slice and handle this exception in a client application.            |
| [Ice Greeter](./Ice/greeter/)                            | Shows how to call a remote Greeter object with Ice. **Start with this demo!**                              |
| [Ice Inheritance](./Ice/inheritance/)                    | Shows the power of interface inheritance in Slice.                                                         |
| [Ice Invocation Timeout](./Ice/invocationTimeout/)       | Shows how to configure invocation timeouts.                                                                |
| [Ice Optional](./Ice/optional/)                          | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.      |
| [Ice Register Communicator](./Ice/registerCommunicator/) | Shows how to register a communicator.                                                                      |
| [IceDiscovery Greeter](./IceDiscovery/greeter/)          | Shows how to configure the IceDiscovery plug-in.                                                           |
| [IceGrid Greeter](./IceGrid/greeter)                     | Shows how to create a simple IceGrid deployment.                                                           |
| [IceGrid LocatorDiscovery](./IceGrid/locatorDiscovery/)  | Shows how to write a client application that configures its locator using the IceLocatorDiscovery plug-in. |
| [IceStorm Weather](./IceStorm/weather/)                  | Shows how to use IceStorm to create a simple pub-sub application.                                          |
