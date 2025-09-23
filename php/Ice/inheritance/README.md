# Ice Inheritance

The Inheritance demo shows how to write a simple filesystem application using interface inheritance.

## Ice prerequisites

- Install the PHP dev kit. See [Ice for PHP installation].

## Building and running the demo

Ice for PHP supports only client-side applications. As a result, you first need to start a Inheritance server
implemented in a language with server-side support, such as Python, Java, or C#.

Then, in a separate window:

- Compile Filesystem.ice with the Slice to PHP compiler into Filesystem.php

```shell
slice2php Filesystem.ice
```

- Run the client application

```shell
php Client.php
```

[Ice for PHP installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-php
