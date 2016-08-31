This demo is a minimal Ice "hello world" application. Each time the
client is run a "sayHello" invocation is sent to the server.

To run the demo, first start the server:

$ java -jar build/libs/server.jar

In a separate window, start the client:

$ java -jar build/libs/client.jar

Note that this demo hardwires port 10000. If port 10000 is not
available on your machine, you need to edit both client and server
to use a free port.

To run the server with WebSocket support:

$ server --Ice.Default.Protocol=ws

To run the client with WebSocket support:

$ client --Ice.Default.Protocol=ws

Also included is a sample [build.gradle](./build.gradle.example) file
that demonstrates how to use the Ice Builder for Gradle plugin and
build a simple Ice application.

**Note:** To run `gradle` from this folder using the minimal example build
file, you need to invoke gradle with the `--no-search-upward` flag or create
a `setting.gradle` file (an empty one is fine).
