This demo is a C++ command-line client for the [ZeroC Chat Demo][1].
It connects to a single chat room, allowing you to chat with other chat
room participants.

By default, the demo connects to the chat server hosted on demo.zeroc.com
through a WebSocket reverse proxy hosted on zeroc.com.

You may run either the Glacier2 client (`chatgl2client`) or the polling client
(`chatpollclient`), as well as any username or password to connect.

If you wish to use your own server from the `server` direcory you should specify
the appropriate custom configuration file when launching the client:

```
chatgl2client --Ice.Config=config.gl2client
chatpollclient --Ice.Config=config.pollclient
```

[1]: https://doc.zeroc.com/display/Doc/Chat+Demo
