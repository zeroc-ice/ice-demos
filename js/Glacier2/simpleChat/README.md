This example presents a very simple chat server to demonstrate the use of a
[Glacier2 session][1].

To run the demo, first follow the instructions from the C++ Glacier2
simpleChat demo README to start the server.

In a separate window, start the client:

```
node Client.js
```

If you plan to run this demo using clients running on different hosts
than the glacier2router, it is necessary to first modify the
configuration. You need to change the `Glacier2.Client.Endpoints`
property in `config.glacier2` and the `Ice.Default.Router` property in
`config.client`. In all cases you must set the host parameter of the
`-h host` endpoint option to the actual parameter with the actual external
address of the machine on which glacier2router is running.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/glacier2/getting-started-with-glacier2
