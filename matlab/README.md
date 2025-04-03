# Ice for MATLAB Demos

This directory contains example Ice applications written in MATLAB. Each example is a simple client application
that demonstrates a specific feature or programming technique. The corresponding server is provided by the C++, C#,
Java, and Python demos.

## MATLAB Demos

|                                            |                                                                                                                                     |
|--------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| [Glacier2 Greeter](./Glacier2/Greeter/)    | Shows how to write a basic Glacier2 client.                                                                                         |
| [Ice Config](./Ice/Config/)                | Shows how to configure a client application using an Ice configuration file.                                                        |
| [Ice Context](./Ice/Context/)              | Shows different ways to set request contexts.                                                                                       |
| [Ice Filesystem](./Ice/Filesystem/)        | Shows the power of interface inheritance in Slice.                                                                                  |
| [Ice Greeter](./Ice/Greeter/)              | Shows how to call a remote Greeter object with Ice. **Start with this demo!**                                                       |
| [Ice Invocation Timeout](./Ice/invocationTimeout/) | Shows how to configure invocation timeouts.                                                                                 |
| [Ice Optional](./Ice/Optional/)            | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.                               |
| [IceDiscovery Greeter](./IceDiscovery/Greeter/) | Shows how to configure the IceDiscovery plugin.                                                                                |
| [IceGrid Greeter](./IceGrid/Greeter)       | Shows how to create a simple IceGrid deployment.                                                                                    |
| [IceGrid LocatorDiscovery](./IceGrid/LocatorDiscovery/) | Shows how to write a client application that configures its locator using the IceLocatorDiscovery plugin.              |
| [IceStorm Weather](./IceStorm/Weather/)    | Shows how to use IceStorm to create a simple pub-sub application.                                                                   |
