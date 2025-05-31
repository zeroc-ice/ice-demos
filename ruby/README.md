# Ice for Ruby Demos

This folder contains example Ice applications written in Ruby. Each example is a simple client application that
demonstrates a specific feature or programming technique.

## Building Requirements

You'll need to install Ice for Ruby in order to use any of the sample programs.

```shell
gem install zeroc-ice --source https://download.zeroc.com/nexus/repository/rubygems-nightly --user --pre
```

# Ruby Demos

|                                            |                                                                                                                                     |
|--------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| [Ice Config](./Ice/config/)                | Shows how to configure a client application using an Ice configuration file.                                                        |
| [Ice Context](./Ice/context/)              | Shows different ways to set request contexts.                                                                                       |
| [Ice CustomError](./Ice/customError/)      | Shows how to define an exception in Slice, and how to throw and catch this exception.                                               |
| [Ice Filesystem](./Ice/filesystem/)        | Shows the power of interface inheritance in Slice.                                                                                  |
| [Ice Greeter](./Ice/greeter/)              | Shows how to call a remote Greeter object with Ice. **Start with this demo!**                                                       |
| [Ice Invocation Timeout](./Ice/invocationTimeout/) | Shows how to configure invocation timeouts.                                                                                 |
| [Ice Optional](./Ice/optional/)            | Shows how to add a field to a Slice class without breaking interop with existing clients and servers.                               |
| [IceDiscovery Greeter](./IceDiscovery/greeter/) | Shows how to configure the IceDiscovery plug-in.                                                                               |
