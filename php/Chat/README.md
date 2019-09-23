This demo is a PHP client for the [ZeroC Chat Demo][1]. It connects to a
single chat room, allowing you to chat with other chat room participants.

By default, the demo connects to the chat server hosted on demo.zeroc.com
through a WebSocket reverse proxy hosted on zeroc.com.

Follow these steps to install the demo:

1) Install the Ice extension and run-time files as described in the
   [manual][2]. Restart the Web server if necessary.

2) Compile all the demo Slice files (see [php/README.md](../../README.md)),
   or compile just this demo's Slice files with `slice2php`:
   ```
   slice2php -I. -I<slice dir of Ice installation> *.ice
   ```

3) Configure your PHP-enabled web server to serve the directory where this
   file exists.

4) Start a Web browser and open the index.html page to begin using the demo.

[1]: https://doc.zeroc.com/technical-articles/general-topics/chat-demo
[2]: https://doc.zeroc.com/rel/ice-releases/ice-3-7/ice-3-7-3-release-notes
