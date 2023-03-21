This demo demonstrates the use of [Glacier2 session helpers][1] and Windows
Presentation Framework (WPF) to create a graphical client for the simple
chat server.

First follow the instructions in the C++ Glacier2 simpleChat demo README
to start the server and the Glacier2 router.

In a separate window, start the WPF client:

```
client
```

If you plan to run this demo using clients running on different hosts
than the glacier2router, it is necessary to first modify the
configuration. You need to change the `Glacier2.Client.Endpoints`
property in `config.glacier2` and the `Ice.Default.Router` property in
`config.client`. In all cases you must set the `-h x.x.x.x` endpoint
parameter with the actual external address of the machine on which
glacier2router is running.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/glacier2/glacier2-sessionhelper-class
