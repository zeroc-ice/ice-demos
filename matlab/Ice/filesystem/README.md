# Ice Filesystem

This demo implements the filesystem application shown at the end of MATLAB mapping chapter in the Ice manual.

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
