# C++ Demos

- [C++ Demos](#c-demos)
  - [Overview](#overview)
  - [macOS Prerequisites](#macos-prerequisites)
  - [Ubuntu Prerequisites](#ubuntu-prerequisites)
  - [RedHat Prerequisites](#redhat-prerequisites)
  - [Windows Prerequisites](#windows-prerequisites)
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
- Nightly build of Ice

Install the nightly build of Ice:

```shell
brew tap zeroc-ice/nightly  https://download.zeroc.com/nexus/repository/nightly/homebrew-nightly.git
brew install zeroc-ice/nightly/ice
```

## Ubuntu Prerequisites

- Ubuntu 24.04
- GCC C++ Toolchain
- CMake
- Nightly build of Ice

Install the nightly build of Ice:

```shell
sudo mkdir -p /etc/apt/keyrings
sudo curl -fsSL https://download.zeroc.com/GPG-KEY-zeroc-nightly \
  | sudo gpg --dearmor -o /etc/apt/keyrings/zeroc-nightly.gpg

echo "deb [signed-by=/etc/apt/keyrings/zeroc-nightly.gpg] https://download.zeroc.com/nexus/repository/ubuntu24.04-nightly nightly main" \
  | sudo tee /etc/apt/sources.list.d/zeroc-nightly.list

sudo apt-get update
sudo apt-get install -y zeroc-ice-all-dev
```

## RedHat Prerequisites

- RedHat 9
- GCC C++ Toolchain
- CMake
- Nightly build of Ice

Install the nightly build of Ice:

```shell
sudo tee /etc/yum.repos.d/ice-nightly.repo <<EOF
[ice-nightly]
name=Ice Nightly Repository
baseurl=https://download.zeroc.com/nexus/repository/rhel9-nightly
enabled=1
gpgcheck=0
EOF

sudo dnf install -y ice-all-devel
```

## Windows Prerequisites

- Visual Studio 2022
- CMake (installed via Visual Studio installer)
- Nightly build of Ice

The CMake script will automatically download the required NuGet packages when you
build the demos. The NuGet packages are cached the [cmake](./cmake) directory.

## C++ Demos

|                                                         |                                                                                                           |
| ------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| [DataStorm Clock](./DataStorm/clock/)                   | Shows how to implement a custom encoder and decoder.                                                      |
| [DataStorm KeyFilter](./DataStorm/keyFilter/)           | Shows how to use a key filters.                                                                           |
| [DataStorm Minimal](./DataStorm/minimal/)               | Shows how to create a minimal DataStorm publisher and subscriber.                                         |
| [DataStorm Node](./DataStorm/node/)                     | Shows the uses of a DataStorm node.                                                                       |
| [DataStorm SampleFilter](./DataStorm/sampleFilter/)     | Shows how to use sample filters.                                                                          |
| [DataStorm Stock](./DataStorm/stock/)                   | Shows how to use partial updates.                                                                         |
| [Glacier2 Callback](./Glacier2/callback/)               | Shows how to write a Glacier2 client and implement callbacks in this client.                              |
| [Ice Bidir](./Ice/bidir/)                               | Shows how to send requests "the other way around", from the server to the client.                         |
| [Ice Callback](./Ice/callback/)                         | Shows how to implement callbacks in a client application.                                                 |
| [Ice Cancellation](./Ice/cancellation/)                 | Shows how to cancel a request and how to configure invocation timeouts.                                   |
| [Ice Config](./Ice/config/)                             | Shows how to configure client and server applications using Ice configuration files.                      |
| [Ice Context](./Ice/context/)                           | Shows how to set and retrieve request contexts.                                                           |
| [Ice Custom Error](./Ice/customError/)                  | Shows how to define a new exception in Slice and return this exception from a Slice operation.            |
| [Ice Filesystem](./Ice/filesystem/)                     | Shows the power of interface inheritance in Slice.                                                        |
| [Ice Forwarder](./Ice/forwarder/)                       | Shows how to write a servant that forwards all requests it receives to another remote Ice object.         |
| [Ice Greeter](./Ice/greeter/)                           | Shows how to call and implement a canonical Greeter application with Ice. **Start with this demo!**       |
| [Ice Middleware](./Ice/middleware/)                     | Shows how to write a simple middleware and how to add this middleware to an object adapter.               |
| [Ice Multicast](./Ice/multicast/)                       | Shows how to use UDP multicast.                                                                           |
| [Ice Optional](./Ice/optional/)                         | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.     |
| [IceBox Greeter](./IceBox/greeter/)                     | Shows how to create an IceBox service.                                                                    |
| [IceDiscovery Greeter](./IceDiscovery/greeter)          | Shows how to use the IceDiscovery plugin.                                                                 |
| [IceDiscovery Replication](./IceDiscovery/replication/) | Shows how to use the IceDiscovery plugin with replicated servers.                                         |
| [IceGrid Greeter](./IceGrid/greeter/)                   | Shows how to create a simple IceGrid deployment.                                                          |
| [IceGrid LocatorDiscovery](./IceGrid/locatorDiscovery/) | Shows how to write a client application that configures its locator using the IceLocatorDiscovery plugin. |
| [IceGrid Query](./IceGrid/query/)                       | Shows how use the Query object provided by the IceGrid registry to lookup a well-known object by type.    |
| [IceStorm Weather](./IceStorm/weather/)                 | Shows how to use IceStorm to create a simple pub-sub application.                                         |
