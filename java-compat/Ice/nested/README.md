A demo to illustrate how [nested callbacks][1] work, and how the size of
the thread pool affects the maximum nesting depth.

To run the demo, first start the server:

```
java -jar build/libs/server.jar
```

In a separate window, start the client:

```
java -jar build/libs/client.jar
```

[1]: https://doc.zeroc.com/ice/3.7/client-server-features/the-ice-threading-model/nested-invocations
