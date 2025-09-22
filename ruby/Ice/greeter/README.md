# Ice Greeter

The Greeter demo illustrates how to write a client application with Ice for Ruby.

## Ice prerequisites

- Install the Ruby dev kit. See [Ice for Ruby installation].

## Building and running the demo

- Compile Greeter.ice with the Slice to Ruby compiler into Greeter.rb

```shell
slice2rb Greeter.ice
```

- Run the client application

```shell
ruby client.rb
```

> [!NOTE]
> Ice for Ruby supports only client-side applications. In this demo, we connect to a Greeter server implemented in a
> language with server-side support, such as C++, C#, Java, Python, or Swift.

[Ice for Ruby installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-ruby
