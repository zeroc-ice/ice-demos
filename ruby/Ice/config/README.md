# Ice Config

This demo shows how to configure a client application using an Ice configuration file.

## Ice prerequisites

- Install the Ruby dev kit. See [Ice for Ruby installation].

## Building and running the demo

Ice for Ruby supports only client-side applications. As a result, you first need to start a Config server implemented
in a language with server-side support (C++, C#, Java, Python, or Swift), for example the
[Python Config server](../../../python/Ice/config): follow the instructions in its README to start this server.

Then, in a separate window:

- Compile Greeter.ice with the Slice to Ruby compiler into Greeter.rb

```shell
slice2rb Greeter.ice
```

- Run the client application

```shell
ruby client.rb
```

[Ice for Ruby installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-ruby
