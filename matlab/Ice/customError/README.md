# Ice Custom Error

The Custom error demo shows how to define an exception in Slice, and how to throw and catch this exception.

A Slice-defined exception should be seen as a custom error carried by the response instead of the expected return
value--there is naturally no throwing across the network.

Ice for MATLAB supports only client-side applications. As a result, you first need to start a Custom Error server
implemented in a language with server-side support, such as Python, Java, or C#.

Then, in the MATLAB console:

- Go to the Ice/customError directory

```shell
cd matlab/Ice/customError
```

- Compile the Greeter.ice file with the Slice compiler for MATLAB

```shell
slice2matlab Greeter.ice
```

- Run the client application

```shell
client
```
