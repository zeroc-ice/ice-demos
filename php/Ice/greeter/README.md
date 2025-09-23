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
> Ice for PHP supports only client-side applications. In this demo, we connect to a Greeter server implemented in a
> language with server-side support, such as C++, C#, Java, Python, or Swift.

[Ice for PHP installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-php
