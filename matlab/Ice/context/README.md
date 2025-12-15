# Ice Context

The Context demo shows the 3 different ways to set a request context in a client.

A request context is a `dictionary<string, string>` carried by all requests. It is empty by default, and the application
is free to set any entry in this dictionary.

> [!NOTE]
> The Ice protocol does not support response contexts, only request contexts.

## Ice prerequisites

- Install the MATLAB dev kit. See [Ice for MATLAB installation].

## Building and running the demo

Ice for MATLAB supports only client-side applications. As a result, you first need to start a Context server implemented
in a language with server-side support, such as Python, Java, or C#.

Then, in the MATLAB console:

- Go to the Ice/context directory

```shell
cd matlab/Ice/context
```

- Compile the Greeter.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Greeter.ice
```

- Run the client application

```shell
client
```

[Ice for MATLAB installation]: https://zeroc.com/ice/downloads/3.8/matlab
