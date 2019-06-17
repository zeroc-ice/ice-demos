A simple callback demo that illustrates how a client can pass a proxy
to a server, invoke an operation in the server, and the [server call
back][1] into an object provided by the client as part of that invocation.

To run the demo, first start the server:

```
java -jar build/libs/server.jar
```

In a separate window, start the client:

```
java -jar build/libs/client.jar
```

[1]: https://doc.zeroc.com/ice/3.7/client-server-features/the-ice-threading-model/nested-invocations
