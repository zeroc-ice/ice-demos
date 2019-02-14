[This demo illustrates the use of [optional class members][1] and
[parameters][2].

First, compile all the demo Slice files (see [php/README.md](../../README.md)),
or compile just this demo's Slice file with `slice2php`:
```
slice2php --output-dir generated *.ice
```

Then, start the optional demo server, using any of the supported
language mappings, on this host. If you want to get started quickly,
we recommend using the Python server.

Finally, if you are using one of the Ice for PHP binary distributions,
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

[1]: https://doc.zeroc.com/ice/latest/the-slice-language/optional-data-members
[2]: https://doc.zeroc.com/ice/latest/language-mappings/php-mapping/client-side-slice-to-php-mapping/php-mapping-for-operations
