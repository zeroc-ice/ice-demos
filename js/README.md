# Ice for JavaScript Demos

This folder contains example Ice applications written in JavaScript. Each example is a simple client-server application
that demonstrates a specific feature or programming technique.

Ice for JavaScript has limited server-side support ([see documentation][1]). As a result, you need to run the servers
implemented in a language that fully supports server-side functionality, such as Python, Java, or C#.

## Building Requirements

1. **Node.js 22**
   Download and install the Node.js 22 from
   [nodejs.org](https://nodejs.org/en/download).

2. **ZeroC NPM Registry**
   The `.npmrc` file include in the demos already adds the ZeroC NPM registry to make
   the ZeroC nightly builds available.

## JavaScript Demos

|                                           |                                                                                                     |
| ----------------------------------------- | --------------------------------------------------------------------------------------------------- |
| [Glacier2 Callback](./Glacier2/callback/) | Shows how to write a Glacier2 client and implement callbacks in this client.                        |
| [Ice Bidir](./Ice/bidir/)                 | Shows how to send requests "the other way around", from the server to the client.                   |
| [Ice Cancellation](./Ice/cancellation/)   | Shows how to cancel a request and how to configure invocation timeouts.                             |
| [Ice Context](./Ice/context/)             | Shows how to set and retrieve request contexts.                                                     |
| [Ice Filesystem](./Ice/filesystem/)       | Shows the power of interface inheritance in Slice.                                                  |
| [Ice Greeter](./Ice/greeter/)             | Shows how to call and implement a canonical Greeter application with Ice. **Start with this demo!** |
| [IceStorm Weather](./IceStorm/Weather/)   | Shows how to use IceStorm to create a simple pub-sub application.                                   |

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/javascript-mapping
