# Ice Greeter

The Greeter demo illustrates how to write a client application with Ice for MATLAB.

## Ice prerequisites

- Install the MATLAB dev kit. See [Ice for MATLAB installation].

## Building and running the demo

- Go to the Ice/greeter directory

```shell
cd matlab/Ice/greeter
```

- Compile the Greeter.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Greeter.ice
```

- Run the client application

```shell
client
```

> [!NOTE]
> Ice for MATLAB supports only client-side applications. In this demo, we connect to a Greeter server implemented in a
> language with server-side support, such as C++, C#, Java, Python, or Swift.

[Ice for MATLAB installation]: https://zeroc.com/ice/downloads/3.8/matlab
