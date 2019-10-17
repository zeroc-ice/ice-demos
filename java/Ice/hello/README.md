This demo illustrates how to invoke ordinary (twoway) operations, as
well as how to make [oneway][1], [datagram][2], [secure][3], and
[batched][4] invocations.

To run the demo, first start the server:

```
java -jar build/libs/server.jar
```

In a separate window, start the client:

```
java -jar build/libs/client.jar
```

To test [timeouts][5] you can use 'T' to set an invocation timeout on the
client proxy and 'P' to set a delayed response in the server to cause a
timeout.

[1]: https://doc.zeroc.com/ice/3.7/client-side-features/oneway-invocations
[2]: https://doc.zeroc.com/ice/3.7/client-side-features/datagram-invocations
[3]: https://doc.zeroc.com/ice/3.7/ice-plugins/icessl
[4]: https://doc.zeroc.com/ice/3.7/client-side-features/batched-invocations
[5]: https://doc.zeroc.com/ice/3.7/client-side-features/invocation-timeouts
