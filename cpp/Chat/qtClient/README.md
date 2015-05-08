This demo is a Qt client for the [ZeroC Chat Demo](https://zeroc.com/chat/index.html). This demo connects to a single chat room, allowing you to chat with other chat room participants.

By default, the demo connects throught a WebSocket proxy hosted on zeroc.com to a chat server hosted on demo.zeroc.com.

If you wish to run your own server you can specify a custom configuration file when launching the application:

```
$ open QtChatDemo.app --Ice.Config=config.client
```
