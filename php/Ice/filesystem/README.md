# Ice Filesystem

This demo implements the filesystem application shown at the end of PHP mapping chapter in the Ice manual.

Ice for PHP supports only client-side applications. As a result, you first need to start a Filesystem server implemented
in a language with server-side support, such as Python, Java, or C#.

Then, in a separate window:

- Compile Filesystem.ice with the Slice to PHP compiler into Filesystem.php

```shell
slice2php Filesystem.ice
```

- Run the client application

```shell
php Client.php
```
