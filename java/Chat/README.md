This demo is a Java client for the [ZeroC Chat Demo][1]. It connects to a
single chat room, allowing you to chat with other chat room participants.

By default, the demo connects to the chat server hosted on demo.zeroc.com
through a WebSocket reverse proxy hosted on zeroc.com.

If you wish to run your own server you can specify a custom configuration
file when launching the application:

```
java -jar Chat.jar --Ice.Config=config.client
```

[1]: https://doc.zeroc.com/technical-articles/general-topics/chat-demo
