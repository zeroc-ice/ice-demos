# C++ Demos

- [C++ Demos](#c-demos)
  - [Overview](#overview)
  - [macOS Prerequisites](#macos-prerequisites)
  - [Linux Prerequisites](#linux-prerequisites)
  - [Windows Prerequisites](#windows-prerequisites)
  - [Building the Demos](#building-the-demos)
  - [C++ Demos](#c-demos-1)

## Overview

This directory contains C++ sample programs for various Ice components. These examples
are provided to get you started on using a particular Ice feature or coding technique.

The subdirectories here correspond directly to Ice components, such as
[IceGrid](./IceGrid) and [Glacier2](./Glacier2).

Refer to the README.md file in each demo directory for usage instructions.

## macOS Prerequisites

- macOS Sequoia
- Xcode
- [Homebrew](https://brew.sh)
- CMake
- [Nightly build of Ice](https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#macos)

## Linux Prerequisites

- A supported Linux Distribution (RHEL 9/10, Amazon Linux 2023, Debian 12/13, Ubuntu 24.04)
- GCC C++ Toolchain
- CMake
- [Nightly build of Ice](https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#linux)

## Windows Prerequisites

- Visual Studio 2022
- CMake (installed via Visual Studio installer)
- [Nightly build of Ice](https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#windows)

The CMake script will automatically download the required NuGet packages when you
build the demos. The NuGet packages are cached in the [cmake](./cmake) directory.

## Building the Demos

Follow the instructions provided in each demo’s README.md file.

## C++ Demos

|                                                         |                                                                                                            |
| ------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- |
| [DataStorm Clock](./DataStorm/clock/)                   | Shows how to implement a custom encoder and decoder.                                                       |
| [DataStorm KeyFilter](./DataStorm/keyFilter/)           | Shows how to use a key filters.                                                                            |
| [DataStorm Minimal](./DataStorm/minimal/)               | Shows how to create a minimal DataStorm publisher and subscriber.                                          |
| [DataStorm Node](./DataStorm/node/)                     | Shows the uses of a DataStorm node.                                                                        |
| [DataStorm SampleFilter](./DataStorm/sampleFilter/)     | Shows how to use sample filters.                                                                           |
| [DataStorm Stock](./DataStorm/stock/)                   | Shows how to use partial updates.                                                                          |
| [Glacier2 Callback](./Glacier2/callback/)               | Shows how to write a Glacier2 client and implement callbacks in this client.                               |
| [Glacier2 Greeter](./Glacier2/greeter/)                 | Shows how to write a basic Glacier2 client.                                                                |
| [Ice Bidir](./Ice/bidir/)                               | Shows how to send requests "the other way around", from the server to the client.                          |
| [Ice Callback](./Ice/callback/)                         | Shows how to implement callbacks in a client application.                                                  |
| [Ice Cancellation](./Ice/cancellation/)                 | Shows how to cancel a request and how to configure invocation timeouts.                                    |
| [Ice Config](./Ice/config/)                             | Shows how to configure client and server applications using Ice configuration files.                       |
| [Ice Context](./Ice/context/)                           | Shows how to set and retrieve request contexts.                                                            |
| [Ice Custom Error](./Ice/customError/)                  | Shows how to define a new exception in Slice and return this exception from a Slice operation.             |
| [Ice Filesystem](./Ice/filesystem/)                     | Shows the power of interface inheritance in Slice.                                                         |
| [Ice Forwarder](./Ice/forwarder/)                       | Shows how to write a servant that forwards all requests it receives to another remote Ice object.          |
| [Ice Greeter](./Ice/greeter/)                           | Shows how to call and implement a canonical Greeter application with Ice. **Start with this demo!**        |
| [Ice Middleware](./Ice/middleware/)                     | Shows how to write a simple middleware and how to add this middleware to an object adapter.                |
| [Ice Optional](./Ice/optional/)                         | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.      |
| [Ice Secure](./Ice/secure/)                             | Shows how to use the Ice::SSL APIs to configure the ssl transport.                                         |
| [IceBox Greeter](./IceBox/greeter/)                     | Shows how to create an IceBox service.                                                                     |
| [IceDiscovery Greeter](./IceDiscovery/greeter)          | Shows how to use the IceDiscovery plug-in.                                                                 |
| [IceDiscovery Replication](./IceDiscovery/replication/) | Shows how to use the IceDiscovery plug-in with replicated servers.                                         |
| [IceGrid Greeter](./IceGrid/greeter/)                   | Shows how to create a simple IceGrid deployment.                                                           |
| [IceGrid IceBox](./IceGrid/icebox/)                     | Shows how to deploy an IceBox server with IceGrid.                                                         |
| [IceGrid LocatorDiscovery](./IceGrid/locatorDiscovery/) | Shows how to write a client application that configures its locator using the IceLocatorDiscovery plug-in. |
| [IceGrid Query](./IceGrid/query/)                       | Shows how use the Query object provided by the IceGrid registry to lookup a well-known object by type.     |
| [IceStorm Weather](./IceStorm/weather/)                 | Shows how to use IceStorm to create a simple pub-sub application.                                          |
