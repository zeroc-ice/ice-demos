# InvocationTimeout

This demo demonstrates how to set a timeout period for a client as well as how to catch a timeout exception.

## Ice prerequisites

- Install the Ruby dev kit. See [Ice for Ruby installation].

## Building and running the demo

Ice for Ruby supports only client-side applications. As a result, you first need to start an Invocation Timeout server
implemented in a language with server-side support (Python or Swift), for example the [Python Invocation Timeout
server](../../../python/Ice/invocation_timeout): follow the instructions in its README to start this server.

Then, in a separate window:

- Compile Greeter.ice with the Slice to Ruby compiler into Greeter.rb

```shell
slice2rb Greeter.ice
```

- Run the client application

```shell
ruby client.rb
```

[Ice for Ruby installation]: https://zeroc.com/ice/downloads/3.8/ruby
