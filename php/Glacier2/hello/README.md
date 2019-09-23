This directory contains a PHP/HTML application that demonstrates how
a PHP session can [register an Ice communicator][1] and use it in
subsequent page requests.

To use the application, a user must first log in by providing any
username and password combination (see `Login.php`). Clicking the
'Login' button causes the application to create a communicator,
establish a Glacier2 session, and redirect the user to the session
page (see `Session.php`). From this page the user can invoke the
'sayHello' operation on a hello server via the Glacier2 session.

As explained in the Ice manual, the ability to register a communicator
for use by a PHP session is only useful when the session's page requests
are serviced by the same web server process. In order to use this demo
effectively, you may need to modify your Web server configuration
(e.g., to use a single persistent process).

Non-persistent Web server models (e.g., CGI) cannot be used with this
demo.

Follow these steps to install the demo:

1) Install the Ice extension and run-time files as described in the
   [manual][2]. Restart the Web server if necessary.

2) Compile all the demo Slice files (see [php/README.md](../../README.md)),
   or compile just this demo's Slice file with `slice2php`:
   ```
   slice2php *.ice
   ```

3) Configure your PHP-enabled web server to serve the directory where this file
   exists.

4) In a command window on the Web server host, start a Glacier2 router
   using the configuration file provided in this directory:
    ```
    glacier2router --Ice.Config=config.glacier2
    ```

5) In a separate command window on the same host, start a hello server.
   You can use a server from any Ice language mapping. If you want to get
   started quickly, we recommend using the Python server.

6) Start a Web browser and open the Login.php page to begin using the
   demo. Note that the Glacier2 configuration uses a session timeout
   of 30 seconds.

If you want to run the Glacier2 router on a different host than the
Web server, you will need to modify the router's endpoint in
`config.glacier2` and `Login.php`. To run the hello server on a different
host, modify the endpoint in `Session.php`.

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/php-mapping/client-side-slice-to-php-mapping/application-notes-for-php
[2]: https://doc.zeroc.com/rel/ice-releases/ice-3-7/ice-3-7-3-release-notes
