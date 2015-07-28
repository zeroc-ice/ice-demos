# Qt Chat Demo

This demo is a Qt client for the [ZeroC Chat Demo](https://zeroc.com/chat/index.html). This demo connects to a single chat room, allowing you to chat with other chat room participants.

By default, the demo connects to the chat server hosted on demo.zeroc.com through a WebSocket reverse proxy hosted on zeroc.com.

To run the client on Windows or Linux:

```
$ QtChatDemo
```

To run the client on OS X:

```
$ open QtChatDemo.app
```

If you wish to use your own server from the `server` directory you should specify the custom configuration file when launching the application. For example:

```
$ QtChatDemo --Ice.Config=config.client
```

## Requirements

### Linux and OS X

The chat demo is compatible with both Qt4 and Qt5.  


### Windows

The chat demo on Windows requires Qt5, Microsoft Visual Studio 2013, the Visual Studio Add-in for Qt5.
