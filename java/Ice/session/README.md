This example demonstrates how to clean up per-client objects through
the use of sessions.

When the client starts, a session object is created through which all
per-client objects are created. The server makes sure the connection
from the client remains open and destroys the session when it detects
the closure of this connection.

Once the session is destroyed, whether by the client destroying the
session upon termination or through the server detecting the closure
of the connection, all per-client objects are cleaned up by the
session object.

To run the demo, first start the server:

```
java -jar build/libs/server.jar
```

In a separate window, start the client:

```
java -jar build/libs/client.jar
```
