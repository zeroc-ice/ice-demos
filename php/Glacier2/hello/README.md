This directory contains a PHP/HTML application that demonstrates how
a PHP session can register an Ice communicator and use it in
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
   [manual][1]. Restart the Web server if necessary.

2) If you have not build the demos yet, please refer to the top level
   [php/README.md](../../README.md) file for build instructions.

3) Install the following files in your Web server's document
   directory:

   * Login.php
   * Session.php
   * Hello.php

   The file `Hello.php` is generated from Hello.ice when you build the
   demos. Verify that the files have appropriate access rights.

4) You may need to edit Login.php and Session.php so that the scripts
   are able to include the Ice run time files. For example, if you
   installed Ice in C:\Ice, add a call to set_include_path as shown
   below:
```
   <?php
   set_include_path(get_include_path() . PATH_SEPARATOR . 'C:/Ice/php');
   require 'Ice.php';
   ...
```
5) In a command window on the Web server host, start a Glacier2 router
   using the configuration file provided in this directory:
```
   % glacier2router --Ice.Config=config.glacier2
```
6) In a separate command window on the same host, start a hello
   server. You can use a server from any Ice language mapping.

7) Start a Web browser and open the Login.php page to begin using the
   demo. Note that the Glacier2 configuration uses a session timeout
   of 30 seconds.

If you want to run the Glacier2 router on a different host than the
Web server, you will need to modify the router's endpoint in
`config.glacier2` and `Login.php`. To run the hello server on a different
host, modify the endpoint in `Session.php`.

[1]: https://doc.zeroc.com/display/Rel/Ice+3.7.1+Release+Notes
