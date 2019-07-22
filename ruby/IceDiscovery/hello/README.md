This demo is the Ice hello world demo configured to use [IceDiscovery][1].

The Ice extension for Ruby currently supports only client-side
functionality, therefore you must use a server from any other language
mapping. If you want to get started quickly, we recommend using the
Python server.

After starting the server, run the Ruby client:

```
ruby client.rb
```

The client uses IceDiscovery to enable the location of the Ice hello
object. See the `config.client` file for details on the IceDiscovery
configuration.

[1]: https://doc.zeroc.com/ice/3.7/ice-plugins/icediscovery
