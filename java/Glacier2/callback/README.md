This example demonstrates the use of several Glacier2 features, including
[callbacks][1] and [request contexts][2].

To run the demo:

Start the server:

```
java -jar build/libs/server.jar
```

In a separate window, start the Glacier2 router:

```
glacier2router --Ice.Config=config.glacier2
```

In a separate window, start the client:

```
java -jar build/libs/client.jar
```

If you plan to run this demo using clients on different hosts than the
router, you must first modify the configuration. You need to change
the `Glacier2.Client.Endpoints` property in `config.glacier2` and the
`Ice.Default.Router` properties in `config.client`. In all cases you must
replace the `-h 127.0.0.1` parameter with the actual external address
of the machine on which glacier2router is running.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/glacier2/callbacks-through-glacier2
[2]: https://doc.zeroc.com/ice/3.7/ice-services/glacier2/how-glacier2-uses-request-contexts
