# Ice bidir

This demo shows how to use [bidirectional connections] for callbacks. This is typically used if the server cannot
open a connection to the client to send callbacks, for example, because firewalls block incoming connections to the
client.

To run the demo, first start the server:

```shell
.build/debug/Server
```

In a separate window, start the client:

```shell
.build/debug/Client
```

[bidirectional connections]: https://doc.zeroc.com/ice/3.7/client-server-features/connection-management/bidirectional-connections
