This demo illustrates how to invoke ordinary (twoway) operations, as
well as how to make [oneway][1], [datagram][2], [secure][3], and
[batched][4] invocations.

To run the demo, first start the server:

```
server
```

In a separate window, start the client:

```
client
```

To test [timeouts][5] you can use 'T' to set an invocation timeout on the
client proxy and 'P' to set a delayed response in the server to cause a
timeout.

[1]: https://doc.zeroc.com/display/IceSwift/Oneway+Invocations
[2]: https://doc.zeroc.com/display/IceSwift/Datagram+Invocations
[3]: https://doc.zeroc.com/display/IceSwift/IceSSL
[4]: https://doc.zeroc.com/display/IceSwift/Batched+Invocations
[5]: https://doc.zeroc.com/display/IceSwift/Invocation+Timeouts
