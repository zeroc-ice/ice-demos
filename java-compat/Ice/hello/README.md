This demo illustrates how to invoke ordinary (twoway) operations, as
well as how to make oneway, datagram, secure, and batched invocations.

To run the demo, first start the server:

```
java -jar build/libs/server.jar
```

In a separate window, start the client:

```
java -jar build/libs/client.jar
```

To test timeouts you can use 'T' to set an invocation timeout on the client
proxy and 'P' to set a delayed response in the server to cause a timeout.
