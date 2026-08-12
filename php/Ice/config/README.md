# Ice Config

This demo shows how to configure a client application using an Ice configuration file. The configuration we
demonstrate sets up SSL: the properties in `client.conf` select `ssl` as the default transport protocol and specify
the certificates used by IceSSL.

## Ice prerequisites

- Install the PHP dev kit. See [Ice for PHP installation].

## Building and running the demo

Ice for PHP supports only client-side applications. As a result, you first need to start a Config server implemented in
a language with server-side support (C++, C#, Java, Python, or Swift), for example the [Python Config
server](../../../python/Ice/config): follow the instructions in its README to start this server.

Then, in a separate window:

- Compile Greeter.ice with the Slice to PHP compiler into Greeter.php

```shell
slice2php Greeter.ice
```

- Run the client application

```shell
php Client.php
```

You can pass `--Ice` command-line options to set additional properties or override the properties set in the
configuration file.

For example:

```shell
php Client.php --Ice.Trace.Network=2
```

[Ice for PHP installation]: https://zeroc.com/ice/downloads/3.8/php
