# Ice Invocation Timeout

The Invocation Timeout demo shows how to use invocation timeouts.

## Ice prerequisites

- Install the PHP dev kit. See [Ice for PHP installation].

## Building and running the demo

Ice for PHP supports only client-side applications. As a result, you first need to start an Invocation Timeout server
implemented in a language with server-side support (Python or Swift), for example the [Python Invocation Timeout
server](../../../python/Ice/invocation_timeout): follow the instructions in its README to start this server.

Then, in a separate window:

- Compile Greeter.ice with the Slice to PHP compiler into Greeter.php

```shell
slice2php Greeter.ice
```

- Run the client application

```shell
php Client.php
```

[Ice for PHP installation]: https://zeroc.com/ice/downloads/3.8/php
