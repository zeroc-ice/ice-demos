# Ice Filesystem

The Filesystem demo shows how to write a simple filesystem application using interface inheritance.

Ice for MATLAB supports only client-side applications. As a result, you first need to start a Filesystem server
implemented in a language with server-side support, such as Python, Java, or C#.

Then, in the MATLAB console:

- Go to the Ice/greeter directory

```shell
cd matlab/Ice/filesystem
```

- Compile the Filesystem.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Filesystem.ice
```

- Run the client application

```shell
client
```
