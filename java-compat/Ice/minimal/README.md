This demo is a minimal Ice "hello world" application. Each time the
client is run a "sayHello" invocation is sent to the server.

To run the demo, first start the server:
```
java -jar build/libs/server.jar
```

In a separate window, start the client:
```
java -jar build/libs/client.jar
```

Note that this demo uses port 10000. If port 10000 is not available on your
machine, you need to edit both client and server to use a free port.

The demo also assumes the client and server are running on the same host.
To run the demo on separate hosts, edit the server to remove `-h localhost`
from the object adapter's endpoint, and edit the client to replace `localhost`
with the host name or IP address of the server.

To run the server with WebSocket support:
```
java -jar build/libs/server.jar --Ice.Default.Protocol=ws
```

To run the client with WebSocket support:
```
java -jar build/libs/client.jar --Ice.Default.Protocol=ws
```

Also included is a sample [build.gradle](./build.gradle.example) file that
demonstrates how to use the Ice Builder for Gradle plug-in to build a simple
Ice application.

**Note:** To run `gradle` from this folder using the example build file, you
need to invoke gradle with the `--no-search-upward` flag or create a
`settings.gradle` file (an empty one is fine).
