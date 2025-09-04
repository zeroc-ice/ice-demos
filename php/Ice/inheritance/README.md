# Ice Inheritance

The Inheritance demo shows how to write a simple filesystem application using interface inheritance.

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
