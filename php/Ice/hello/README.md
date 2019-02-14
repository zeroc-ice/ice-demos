This directory contains a PHP/HTML implementation of the Ice hello
client.

Follow these steps to install the demo:

1) Install the Ice extension and run-time files as described in the
   [manual][1]. Restart the Web server if necessary.

2) If you have not built the demos yet, please refer to the top level
   [php/README.md](../../README.md) file for build instructions.

3) Configure your PHP-enabled web server to serve the directoy where this
   file exists.

4) In a command window on the Web server host, start a hello server.
   You can use a server from any Ice language mapping. If you want to
   get started quickly, we recommend using the Python server.

5) Start a Web browser and open the `Client.php` page to begin using the
   demo. Note that support for secure invocations via SSL are disabled
   by default. If you have configured SSL for the Ice extension, edit
   `Client.php` and set the `have_ssl` variable to true.

[1]: https://doc.zeroc.com/display/Rel/Ice+3.7.2+Release+Notes
