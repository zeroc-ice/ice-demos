# Ice Invocation Timeout

The Invocation Timeout demo shows how to use invocation timeouts.

## Ice prerequisites

- Install the PHP dev kit. See [Ice for PHP installation].

## Building and running the demo

Ice for PHP supports only client-side applications. As a result, you first need to start a Greeter server implemented
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

[Ice for PHP installation]: https://zeroc.com/ice/downloads/3.8/php
