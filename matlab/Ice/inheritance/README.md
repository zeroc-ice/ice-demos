# Ice Inheritance

The Inheritance demo shows how to write a simple filesystem application using interface inheritance.

## Ice prerequisites

- Install the MATLAB dev kit. See [Ice for MATLAB installation].

## Building and running the demo

Ice for MATLAB supports only client-side applications. As a result, you first need to start an Inheritance server
implemented in a language with server-side support, such as Python, Java, or C#.

Then, in the MATLAB console:

- Go to the Ice/inheritance directory

```shell
cd matlab/Ice/inheritance
```

- Compile the Filesystem.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Filesystem.ice
```

- Run the client application

```shell
client
```

[Ice for MATLAB installation]: https://zeroc.com/ice/downloads/3.8/matlab
