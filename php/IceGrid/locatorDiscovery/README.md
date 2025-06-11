# IceGrid Locator Discovery

The LocatorDiscovery demo shows how to write a client application that configures its locator using the
LocatorDiscovery plug-in.

Ice for PHP supports only client-side applications. As a result, you first need to deploy the IceGrid
Greeter server implemented in a language with server-side support, such as Python, Java, or C#.

Then, in a new window:

- Go to the IceGrid/locatorDiscovery directory

```shell
cd php/IceGrid/locatorDiscovery
```

- Compile the Greeter.ice file with the Slice compiler for PHP

```shell
slice2php Greeter.ice
```

- Run the client application

```shell
php Client.php
```
