This demo shows how to implement a [custom locator][1] that delegates
to the [IceGrid][2] registry.

To run the demo, first start the IceGrid service:

```
icegridnode --Ice.Config=config.grid
```

In a separate window start the custom locator:

```
locator
```

Now in another window deploy the application:

```
icegridadmin --Ice.Config=config.grid -e "application add application.xml"
```

The custom locator included in this demo only allows clients that set a
specific value in the Locator proxy context to obtain a proxy to the server
object.

To run the client without setting this value:

```
client
```

To run the client with the context set properly:

```
client --context
```

[1]: https://doc.zeroc.com/ice/3.7/client-server-features/locators
[2]: https://doc.zeroc.com/ice/3.7/ice-services/icegrid
