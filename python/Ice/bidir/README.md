This demo shows how to use [bidirectional connections][1] for callbacks.
This is typically used if the server cannot open a connection to the
client to send callbacks, for example, because firewalls block
incoming connections to the client.

To run the demo, first start the server:

```
python server.py
```

In a separate window, start the client:

```
python client.py
```

[1]: https://doc.zeroc.com/ice/4.0/client-server-features/connection-management/bidirectional-connections
