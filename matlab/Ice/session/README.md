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

Ice for MATLAB currently supports only client-side functionality, therefore
you must use a server from any other language mapping. If you want to get
started quickly, we recommend using the Python server.

After starting the server, run the MATLAB client:

```
>> client
```
