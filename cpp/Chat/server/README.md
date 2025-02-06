# Chat Server

This demo is the server for the [ZeroC Chat Demo][1].

The chat demo server implements two different session systems, one using
Glacier2 sessions and callbacks, and the other one using a custom session
mechanism for Ice clients that cannot use callbacks.

The Slice definitions for the chat demo are:

- `slice/ChatSession.ice`: Definitions for Glacier2 clients (C++, Java, .NET,
  `JavaScript)

- `slice/PollingChat.ice`: Definitions for non-Glacier2 clients (PHP)

You can use this demo if you want to host your own Chat Demo server.

To build the demo run:

```shell
cmake -B build -S .
cmake --build build --config Release
```

To run the demo, first start the server:

**Linux/macOS:**

```shell
./build/chatserver --Ice.Config=config.chatserver
```

**Windows:**

```shell
build\Release\chatserver --Ice.Config=config.chatserver
```

For push clients Glacier2 is also required, to start it run the following in a
new shell:

```shell
glacier2router --Ice.Config=config.glacier2router
```

[1]: https://doc.zeroc.com/display/Doc/Chat+Demo
