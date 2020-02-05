This demo is the Ice hello world demo configured to use [IceDiscovery][1].

To run the demo, first start the server:

```
python server.py
```

In a separate window, start the client:

```
python client.py
```

The client and server use IceDiscovery to enable the location of the
Ice hello object. See the `config.server` and `config.client` files for
details on the IceDiscovery configuration.

[1]: https://doc.zeroc.com/ice/4.0/ice-plugins/icediscovery
