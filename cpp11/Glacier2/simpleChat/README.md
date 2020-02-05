This example presents a very simple chat server to demonstrate the use of a
[Glacier2 session][1].

To run the demo, first start the chat server:

```
server
```

In a separate window, start the Glacier2 router:

```
glacier2router --Ice.Config=config.glacier2
```

In a separate window, start the client:

```
client
```

If you plan to run this demo using clients running on different
hosts than the glacier2router, it is necessary to first modify the
configuration. You need to change the `Glacier2.Client.Endpoints`
property in `config.glacier2` and the `Ice.Default.Router` and
`Chat.Client.Router` properties in config. In all cases you must
replace the `-h 127.0.0.1` parameter with the actual external address
of the machine on which glacier2router is running.

When using Glacier2 session, the Glacier2 router destroys your
sessions when a client goes away and otherwise becomes unresponsive.
Your implementation of `Glacier2::Session::destroy` can then clean
up resources associated with this client.

If the Glacier2 router itself dies or becomes unreachable, you won't
get such a notification. This demo shows how to handle this situation
as well: the ChatRoom in `ChatSessionI.cpp` keeps track of the
sessions associated with each connection from a Glacier2 router,
and destroys these sessions when the connection is closed. You can
kill the Glacier2 router to see this cleanup occur.

[1]: https://doc.zeroc.com/ice/4.0/ice-services/glacier2/getting-started-with-glacier2
