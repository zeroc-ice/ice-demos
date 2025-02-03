# Greeter

The Greeter demo illustrates how to write a client application with Ice for Ruby.

Ice for Ruby supports only client-side applications. As a result, you need to first start a Greeter server implemented
in a language with server-side support, such as Python, Java or C#.

Then, in a separate window:

- compile Greeter.ice with the Slice to Ruby compiler into Greeter.rb

```shell
slice2rb Greeter.ice
```

- run the client application

```shell
ruby client.rb
```
