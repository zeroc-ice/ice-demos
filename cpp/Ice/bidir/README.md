This demo shows how to use [bidirectional connections][1] for callbacks.
This is typically used if the server cannot open a connection to the
client to send callbacks, for example, because firewalls block
incoming connections to the client.

To run the demo, first start the server:

```
server
```

In a separate window, start the client:

```
client
```

[1]: https://doc.zeroc.com/ice/3.7/client-server-features/connection-management/bidirectional-connections
