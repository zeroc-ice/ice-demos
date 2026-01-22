# Ice minimal

This demo is a minimal Ice "hello world" application. Each time the client is run a "sayHello" invocation is sent to
the server.

## Building the Demo

```shell
make slice
swift build
```

## Running the Demo

First start the server:
```shell
.build/debug/Server
```

In a separate window, start the client:
```shell
.build/debug/Client
```

Note that this demo uses port 10000. If port 10000 is not available on your machine, you need to edit both client and
server to use a free port.

The demo also assumes the client and server are running on the same host. To run the demo on separate hosts, edit the
server to remove `-h localhost` from the object adapter's endpoint, and edit the client to replace `localhost` with the
host name or IP address of the server.

To run the server with WebSocket support:
```shell
.build/debug/Server --Ice.Default.Protocol=ws
```

To run the client with WebSocket support:
```shell
.build/debug/Client --Ice.Default.Protocol=ws
```
