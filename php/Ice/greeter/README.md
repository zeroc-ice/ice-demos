# Ice Greeter

The Greeter demo illustrates how to write a client application with Ice for PHP.

Follow these steps:

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
