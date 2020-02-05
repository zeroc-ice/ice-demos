This demo shows how to use [bidirectional connections][1] for callbacks.
This is typically used if the server cannot open a connection to the
client to send callbacks, for example, because firewalls block
incoming connections to the client.

To run the demo, first you need to start an Ice bidir server from
another language mapping (Java, C++, C#, or Python). If you want to
get started quickly, we recommend using the Python server.

After starting the server, open a separate window and start the
client:

```
node Client.js
```

[1]: https://doc.zeroc.com/ice/4.0/client-server-features/connection-management/bidirectional-connections
