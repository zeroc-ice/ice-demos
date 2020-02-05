A simple callback demo that illustrates how a client can pass a proxy
to a server, invoke an operation in the server, and the [server call
back][1] into an object provided by the client as part of that invocation.

To run the demo, first start the server:

```
python server.py
```

In a separate window, start the client:

```
python client.py
```

[1]: https://doc.zeroc.com/ice/4.0/client-server-features/the-ice-threading-model/nested-invocations
