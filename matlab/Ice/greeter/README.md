# Ice Greeter

The Greeter demo illustrates how to write a client application with Ice for MATLAB.

## Ice prerequisites

- Install the MATLAB dev kit. See [Ice for MATLAB installation].

## Building and running the demo

- Go to the matlab/Ice/greeter directory

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
> Ice for MATLAB supports only client-side applications. By default, the client connects to a Greeter server hosted by
> ZeroC at `hello.zeroc.com`; as a result, you don't need to start a Greeter server to run this demo. If you want to
> use your own Greeter server (implemented in a language with server-side support, such as C++, C#, Java, Python, or
> Swift), replace `hello.zeroc.com` with `localhost` in the proxy string in `client.m`.

[Ice for MATLAB installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-matlab
