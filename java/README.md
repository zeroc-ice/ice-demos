# Ice for Java Demos

This folder contains example Ice applications written in Java. Each example is a simple client-server application that
demonstrates a specific feature or programming technique.

The Gradle projects download and install the Ice for Java packages as needed. See [Ice for Java installation] for
details.

|                                               |                                                                                                     |
|-----------------------------------------------|-----------------------------------------------------------------------------------------------------|
| [Glacier2 Greeter](./Glacier2/greeter/)       | Shows how to write a basic Glacier2 client.                                                         |
| [Ice Android Greeter](./Ice/android-greeter/) | Shows how to create an Ice for Java Android application.                                            |
| [Ice Bidir](./Ice/bidir/)                     | Shows how to send requests "the other way around", from the server to the client.                   |
| [Ice Callback](./Ice/callback/)               | Shows how to implement callbacks in a client application.                                           |
| [Ice Cancellation](./Ice/cancellation/)       | Shows how to cancel a request and how to configure invocation timeouts.                             |
| [Ice Config](./Ice/config/)                   | Shows how to configure client and server applications using Ice configuration files.                |
| [Ice Context](./Ice/context/)                 | Shows how to set and retrieve request contexts.                                                     |
| [Ice Custom Error](./Ice/customError/)        | Shows how to define a new exception in Slice and return this exception from a Slice operation.      |
| [Ice Forwarder](./Ice/forwarder/)             | Shows how to write a servant that forwards all requests it receives to another remote Ice object.   |
| [Ice Greeter](./Ice/greeter/)                 | Shows how to call and implement a canonical Greeter application with Ice. **Start with this demo!** |
| [Ice Inheritance](./Ice/inheritance/)         | Shows the power of interface inheritance in Slice.                                                  |
| [Ice Middleware](./Ice/middleware/)           | Shows how to write a simple middleware and how to add this middleware to an object adapter.         |
| [Ice Secure](./Ice/secure/)                   | Shows how to use com.zeroc.Ice.SSL APIs to configure the ssl transport.                             |
| [IceStorm Weather](./IceStorm/weather/)       | Shows how to use IceStorm to create a simple pub-sub application.                                   |

[Ice for Java installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-java
