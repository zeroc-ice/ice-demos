# Ice for MATLAB Demos

This directory contains example Ice applications written in MATLAB. Each example is a simple client application
that demonstrates a specific feature or programming technique. The corresponding server is provided by the C++, C#,
Java, and Python demos.

## Building Requirements

1. **MATLAB R2024a**
   [MATLAB](https://www.mathworks.com/products/matlab.html).

2. **Ice for MATLAB nightly toolbox**
   [Ice for MATLAB nightly builds](https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#matlab)

## MATLAB Demos

|                                                         |                                                                                                            |
|---------------------------------------------------------|------------------------------------------------------------------------------------------------------------|
| [Glacier2 Greeter](./Glacier2/greeter/)                 | Shows how to write a basic Glacier2 client.                                                                |
| [Ice Cancellation](./Ice/cancellation/)                 | Shows how to cancel a request and how to configure invocation timeouts.                                    |
| [Ice Config](./Ice/config/)                             | Shows how to configure a client application using an Ice configuration file.                               |
| [Ice Context](./Ice/context/)                           | Shows different ways to set request contexts.                                                              |
| [Ice Custom Error](./Ice/customError/)                  | Shows how to define a new exception in Slice and handle this exception in a client application.            |
| [Ice Filesystem](./Ice/filesystem/)                     | Shows the power of interface inheritance in Slice.                                                         |
| [Ice Greeter](./Ice/greeter/)                           | Shows how to call a remote Greeter object with Ice. **Start with this demo!**                              |
| [Ice Optional](./Ice/optional/)                         | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.      |
| [IceDiscovery Greeter](./IceDiscovery/greeter/)         | Shows how to configure the IceDiscovery plug-in.                                                           |
| [IceGrid Greeter](./IceGrid/greeter)                    | Shows how to create a simple IceGrid deployment.                                                           |
| [IceGrid LocatorDiscovery](./IceGrid/locatorDiscovery/) | Shows how to write a client application that configures its locator using the IceLocatorDiscovery plug-in. |
| [IceStorm Weather](./IceStorm/weather/)                 | Shows how to use IceStorm to create a simple pub-sub application.                                          |
