# InvocationTimeout

This demo demonstrates how to set a timeout period for a client as well as how to catch a timeout exception.

Ice for Ruby supports only client-side applications. As a result, you first need to start an InvocationTimeout server
implemented in a language with server-side support, such as Python, Java, or C#.

Then, in a separate window:

- Compile Greeter.ice with the Slice to Ruby compiler into Greeter.rb

```shell
slice2rb Greeter.ice
```

- Run the client application

```shell
ruby client.rb
```