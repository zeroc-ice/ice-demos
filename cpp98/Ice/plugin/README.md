This demo illustrates how to write and configure a simple Ice plug-in
as well as an Ice Logger plug-in.

To run the demo, first start the server:
```
server
```

In a separate window, start the client:
```
client
```

Both the client and server use a custom logger that is loaded as
a logger plug-in. The server also uses a plug-in to implement the
servant for this demo.

Please review the client and sever configuration files to see the
plug-in configuration.
