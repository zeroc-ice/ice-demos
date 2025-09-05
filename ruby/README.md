# Ice for Ruby Demos

This folder contains example Ice applications written in Ruby. Each example is a simple client application that
demonstrates a specific feature or programming technique.

> Ice for Ruby is supported on Linux and macOS platforms.

## Building Requirements

The demos in the main branch require the latest Ice for Ruby nightly build. To install it, run:

```shell
MAKEFLAGS=-j12 gem install zeroc-ice --source https://download.zeroc.com/nexus/repository/rubygems-nightly --user \
    --pre --version '>= 3.8.0.pre'
```

## Ruby Demos

|                                                         |                                                                                                            |
|---------------------------------------------------------|------------------------------------------------------------------------------------------------------------|
| [Glacier2 greeter](./Glacier2/greeter/)                 | Shows how to write a basic Glacier2 client.                                                                |
| [Ice Config](./Ice/config/)                             | Shows how to configure a client application using an Ice configuration file.                               |
| [Ice Context](./Ice/context/)                           | Shows different ways to set request contexts.                                                              |
| [Ice CustomError](./Ice/customError/)                   | Shows how to define an exception in Slice, and how to throw and catch this exception.                      |
| [Ice Greeter](./Ice/greeter/)                           | Shows how to call a remote Greeter object with Ice. **Start with this demo!**                              |
| [Ice Inheritance](./Ice/inheritance/)                   | Shows the power of interface inheritance in Slice.                                                         |
| [Ice Invocation Timeout](./Ice/invocationTimeout/)      | Shows how to configure invocation timeouts.                                                                |
| [Ice Optional](./Ice/optional/)                         | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.      |
| [IceDiscovery Greeter](./IceDiscovery/greeter/)         | Shows how to configure the IceDiscovery plug-in.                                                           |
| [IceGrid Greeter](./IceGrid/greeter)                    | Shows how to create a simple IceGrid deployment.                                                           |
| [IceGrid LocatorDiscovery](./IceGrid/locatorDiscovery/) | Shows how to write a client application that configures its locator using the IceLocatorDiscovery plug-in. |
| [IceStorm Weather](./IceStorm/weather/)                 | Shows how to use IceStorm to create a simple pub-sub application.                                          |
