# Ice for PHP Demos

This directory contains example Ice applications written in PHP. Each example is a simple client application
that demonstrates a specific feature or programming technique. The corresponding server is provided by the C++, C#,
Java, and Python demos.

## PHP Demos

|                                                    |                                                                                                       |
|----------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| [Glacier2 Greeter](./Glacier2/greeter/)            | Shows how to write a basic Glacier2 client.                                                           |
| [Ice Config](./Ice/config/)                        | Shows how to configure a client application using an Ice configuration file.                          |
| [Ice Context](./Ice/context/)                      | Shows different ways to set request contexts.                                                         |
| [Ice Custom Error](./Ice/customError/)             | Shows how to define a new exception in Slice and handle this exception in a client application.       |
| [Ice Filesystem](./Ice/filesystem/)                | Shows the power of interface inheritance in Slice.                                                    |
| [Ice Greeter](./Ice/greeter/)                      | Shows how to call a remote Greeter object with Ice. **Start with this demo!**                         |
| [Ice Invocation Timeout](./Ice/invocationTimeout/) | Shows how to configure invocation timeouts.                                                           |
| [Ice Optional](./Ice/optional/)                    | Shows how to add a field to a Slice class without breaking interop with existing clients and servers. |
| [IceDiscovery Greeter](./IceDiscovery/greeter/)    | Shows how to configure the IceDiscovery plug-in.                                                      |
