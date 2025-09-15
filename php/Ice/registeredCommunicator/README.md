# Ice Registered Communicator

This demo shows how to use registered communicators with Ice for PHP.

Registered communicators allow reusing a communicator instance across PHP requests. This avoids the overhead of creating
and destroying a communicator for each request. In addition, it allows connections established by the communicator to
remain open and be reused across requests.

This technique is especially useful when PHP is running behind a web server such as Nginx with PHP-FPM (FastCGI Process
Manager), where multiple requests are handled by long-lived worker processes.

## Demo Overview

This demo consists of a PHP-based web application that acts as a client to a remote Ice Greeter server.

The web application is served by Nginx and uses PHP-FPM to handle requests. It demonstrates how to use Ice’s registered
communicator feature in a typical PHP web deployment. This feature allows the PHP client (the web app itself) to reuse
the communicator and its network connections across multiple requests.

## Running the demo

A working **Docker** installation is required for running this demo.

Ice for PHP supports only client-side applications. As a result, you first need to start a Greeter server implemented
in a language with server-side support, such as Python, Java, or C#.

Then, in a separate window:

- Generate PHP Slice definitions
  Compile Greeter.ice with the Slice to PHP compiler into Greeter.php

  ```shell
  slice2php Greeter.ice --output-dir web
  ```

- Run the web server using Docker Composer

  ```shell
  docker-compose up
  ```

- Open your Web Browser
  Navigate to: http://localhost:8080

> [!NOTE]
> Passing `--Ice.Trace.Network` command-line option to the server turns on Network tracing. For this demo, it shows
> you that the client reuses the connection for different request, which means the same communicator is used for these
> requests.
