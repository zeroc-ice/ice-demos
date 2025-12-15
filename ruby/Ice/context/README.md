# Context

The Context demo shows the 3 different ways to set a request context in a client.

A request context is a `dictionary<string, string>` carried by all requests. It is empty by default, and the application
is free to set any entry in this dictionary.

> [!NOTE]
> The Ice protocol does not support response contexts, only request contexts.

## Ice prerequisites

- Install the Ruby dev kit. See [Ice for Ruby installation].

## Building and running the demo

Ice for Ruby supports only client-side applications. As a result, you first need to start a Context server implemented
in a language with server-side support, such as Python, Java, or C#.

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
