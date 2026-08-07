# Ice Greeter

The Greeter demo illustrates how to write a client application with Ice for PHP.

## Ice prerequisites

- Install the PHP dev kit. See [Ice for PHP installation].

## Building and running the demo

- Compile Greeter.ice with the Slice to PHP compiler into Greeter.php

```shell
slice2php Greeter.ice
```

- Run the client application

```shell
php Client.php
```

> [!NOTE]
> Ice for PHP supports only client-side applications. By default, the client connects to a Greeter server hosted by
> ZeroC at `hello.zeroc.com`; as a result, you don't need to start a Greeter server to run this demo. If you want to
> use your own Greeter server (implemented in a language with server-side support, such as C++, C#, Java, Python, or
> Swift), edit the proxy string in `Client.php`: replace `hello.zeroc.com` with `localhost` for a server running on the
> same computer, or with the server's hostname or IP address for a server running on another computer.

[Ice for PHP installation]: https://zeroc.com/ice/downloads/3.8/php
