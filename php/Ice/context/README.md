# Ice Context

The Context demo shows the 2 different ways to set a request context in a client.

A request context is a `dictionary<string, string>` carried by all requests. It is empty by default, and the application
is free to set any entry in this dictionary.

The Ice for PHP extension doesn't support `Ice.ImplicitContext` which is a third way to set the Ice.Context supported
in other languages.

> [!NOTE]
> The Ice protocol does not support response contexts, only request contexts.

## Ice prerequisites

- Install the PHP dev kit. See [Ice for PHP installation].

## Building and running the demo

Ice for PHP supports only client-side applications. As a result, you first need to start a Context server implemented
in a language with server-side support, such as Python, Java, or C#.

Then, in a separate window:

- Compile Greeter.ice with the Slice to PHP compiler into Greeter.php

```shell
slice2php Greeter.ice
```

- Run the client application

```shell
php Client.php
```

[Ice for PHP installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-php
