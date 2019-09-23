This directory contains a PHP/HTML implementation of the Ice hello
client configured to use [IceDiscovery][1].

Follow these steps to install the demo:

1) Install the Ice extension and run-time files as described in the
   [manual][2]. Restart the Web server if necessary.

2) Compile all the demo Slice files (see [php/README.md](../../README.md)),
   or compile just this demo's Slice file with `slice2php`:
   ```
   slice2php *.ice
   ```

3) Configure your PHP-enabled web server to serve the directory where this
   file exists.

4) In a command window on the Web server host, start IceDiscovery hello
   server. You can use a server from any Ice language mapping. If you want
   to get started quickly, we recommend using the Python server.

5) Start a Web browser and open the `Client.php` page to begin using the
   demo. Note that support for secure invocations via SSL are disabled
   by default. If you have configured SSL for the Ice extension, edit
   `Client.php` and set the `have_ssl` variable to `true`.

[1]: https://doc.zeroc.com/ice/3.7/ice-plugins/icediscovery
[2]: https://doc.zeroc.com/rel/ice-releases/ice-3-7/ice-3-7-3-release-notes
