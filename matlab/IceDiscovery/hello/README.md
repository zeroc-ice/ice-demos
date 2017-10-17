This demo is the Ice hello world demo.

Ice for MATLAB currently supports only client-side functionality, therefore
you must use a server from any other language mapping.

After starting the server, run the MATLAB client:

```
Client.main()
```

The client use IceDiscovery to enable the location of the
Ice hello object. See the `config.client` file for
details on the IceDiscovery configuration.
