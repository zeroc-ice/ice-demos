# Config

This demo shows how to configure a client application using an Ice configuration file.

Ice for Ruby supports only client-side applications. As a result, you first need to start a Config server implemented
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
