This directory contains a PHP/HTML implementation of the Ice hello
client.

Follow these steps to install the demo:

1) Install the Ice extension and run-time files as described in the
   [manual][1]. Restart the Web server if necessary.

2) If you have not build the demos yet, please refer to the top level
   [php/README.md](../../README.md) file for build instructions.

3) Install the following files in your Web server's document
   directory:

   * `Client.php`
   * `Hello.php`

   The file `Hello.php` is generated from `Hello.ice` when you build the
   demos. Verify that the files have appropriate access rights.

4) You may need to edit `Client.php` so that the script is able to
   include the Ice run time files. For example, if you installed Ice
   in `C:\Ice`, add a call to `set_include_path` as shown below:
```
   <?php
   set_include_path(get_include_path() . PATH_SEPARATOR . 'C:/Ice/php');
   require 'Ice.php';
   ...
```
5) In a command window on the Web server host, start a hello server.
   You can use a server from any Ice language mapping.

6) Start a Web browser and open the client page to begin using the
   demo. Note that support for secure invocations via SSL are disabled
   by default. If you have configured SSL for the Ice extension, edit
   `client.php` and set the `have_ssl` variable to true.

[1]: https://doc.zeroc.com/display/Ice37/Using+the+Linux+Binary+Distributions#UsingtheLinuxBinaryDistributions-PHP
