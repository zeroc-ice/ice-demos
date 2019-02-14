[This demo illustrates the use of [optional class members][1] and
[parameters][2].

If you have not built the demos yet, please refer to the top level
[php/README.md](../../README.md) file for build instructions.

First, start the optional demo server, using any of the supported
language mappings, on this host. If you want to get started quickly,
we recommend using the Python server.

Then, if you are using one of the Ice for PHP binary distributions,
run the PHP client:

```
php -f Client.php
```

Otherwise, review the comments in the `php.ini` file to determine whether
any changes are needed for your environment, and then run the PHP
client:

```
php -c php.ini -f Client.php
```

[1]: https://doc.zeroc.com/display/Ice37/Optional+Data+Members
[2]: https://doc.zeroc.com/display/Ice37/PHP+Mapping+for+Operations
