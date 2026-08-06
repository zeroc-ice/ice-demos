# IceGrid Locator Discovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plug-in.

## Ice prerequisites

- Install the PHP dev kit. See [Ice for PHP installation].

## Building and running the demo

Ice for PHP supports only client-side applications. As a result, you first need to deploy the IceGrid Greeter server
implemented in a language with server-side support (C++, C#, Java, Python, or Swift), for example the [Python IceGrid
Greeter demo](../../../python/IceGrid/greeter): follow the instructions in its README to deploy this server.

Then, in a new window:

- Compile the Greeter.ice file with the Slice compiler for PHP

```shell
slice2php Greeter.ice
```

- Run the client application

```shell
php Client.php
```

[Ice for PHP installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-php
