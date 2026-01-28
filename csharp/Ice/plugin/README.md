This demo illustrates how to write and configure a simple Ice [plug-in] as well as an Ice [Logger plug-in].

To run the demo, first start the server:

```shell
dotnet server.dll
```

In a separate window, start the client:

```shell
dotnet client.dll
```

Both the client and server use a custom logger that is loaded as a logger plug-in. The server also uses a plug-in to
implement the servant for this demo.

Please review the client and server configuration files to see the plug-in configuration.

[plug-in]: https://archive.zeroc.com/ice/3.7/communicator-and-other-core-local-features/plug-in-facility
[Logger plug-in]: https://archive.zeroc.com/ice/3.7/administration-and-diagnostics/logger-facility/logger-plug-ins
