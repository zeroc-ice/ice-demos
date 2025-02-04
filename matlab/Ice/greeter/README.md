# Greeter

The Greeter demo illustrates how to write a client application with Ice for MATLAB.

Ice for MATLAB supports only client-side applications. As a result, you first need to start a Greeter server implemented
in a language with server-side support, such as Python, Java, or C#.

Then, in the MATLAB console:

- Go to the Ice/greeter directory

```shell
cd matlab/Ice/greeter
```

- Compile the Greeter.ice file with the Slice compiler for MATLAB

```shell
mkdir generated
slice2matlab '--output-dir generated Greeter.ice'
```

- Run the client application

```shell
client
```
