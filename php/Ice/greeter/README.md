# Greeter

The Greeter demo illustrates how to write a client application with Ice for PHP.

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
