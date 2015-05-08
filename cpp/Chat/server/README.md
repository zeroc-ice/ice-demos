This demo is the server for the [ZeroC Chat Demo](https://zeroc.com/chat/index.html).

The chat demo server implements two different session systems, one
using Glacier2 sessions and callbacks, and the other one using a
custom session mechanism for Ice clients that cannot use callbacks.

The Slice definitions for the chat demo are:

slice/ChatSession.ice

    Definitions for Glacier2 clients (C++, Java, .NET, Silverlight)

slice/PollingChat.ice

    Definitions for non-Glacier2 clients (PHP)

The server source code is located in the server subdirectory.

You can use this if you want to host your own Chat Demo server.

To run the server:

```
$ chatserver --Ice.Config=config.chatserver
```

If you require Glacier2 run the following in a new shell:

```
$ glacier2router --Ice.Config=config.glacier2router
```
