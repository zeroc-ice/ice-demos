# Ice for Python Demos

This folder contains example Ice applications written in Python. Each example is a simple client-server application that
demonstrates a specific feature or programming technique.

## Building Requirements

1. **Python**
   Python 3.12 or later. You can download Python from [python.org](https://www.python.org/downloads/)

2. **ZeroC Nightly Pypi Respository**
   The projects are setup to use Ice for Python nightly builds from the ZeroC nightly PYPI repository.
   See [Installing Nightly Builds](https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#python) for setup instructions.

## Building the demos

Follow the instructions provided in each demo’s README.md file.

## Python Demos

|                                                      |                                                                                                       |
|------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| [Glacier2 callback](./Glacier2/callback/)            | Shows how to write a Glacier2 client and implement callbacks in this client.                          |
| [Glacier2 greeter](./Glacier2/greeter/)              | Shows how to write a basic Glacier2 client.                                                           |
| [Ice Bidir](./Ice/bidir/)                            | Shows how to send requests "the other way around", from the server to the client.                     |
| [Ice Callback](./Ice/callback/)                      | Shows how to implement callbacks in a client application.                                             |
| [Ice Config](./Ice/config/)                          | Shows how to configure client and server applications using Ice configuration files.                  |
| [Ice Context](./Ice/context/)                        | Shows how to set and retrieve request contexts.                                                       |
| [Ice Filesystem](./Ice/filesystem/)                  | Shows the power of interface inheritance in Slice.                                                    |
| [Ice Greeter](./Ice/greeter/)                        | Shows how to call and implement a canonical Greeter application with Ice. **Start with this demo!**   |
| [Ice Invocation Timeout](./Ice/invocation_timeout/) | Shows how to use invocation timeouts                                                                  |
| [Ice Optional](./Ice/optional/)                      | Shows how to add a field to a Slice class without breaking interop with existing clients and servers. |
| [IceBox Greeter](./IceBox/Greeter/)                  | Shows how to create an IceBox service.                                                                |
| [IceDiscovery Greeter](./IceDiscovery/greeter/)      | Shows how to configure the IceDiscovery plug-in.                                                      |
| [IceGrid Greeter](./IceGrid/greeter)                 | Shows how to create a simple IceGrid deployment.                                                      |
| [IceStorm Weather](./IceStorm/weather/)              | Shows how to use IceStorm to create a simple pub-sub application.                                     |
