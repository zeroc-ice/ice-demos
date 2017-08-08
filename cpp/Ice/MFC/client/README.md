The client-side portion of the MFC hello demo. Note that this demo can
be used with the MFC server, or any of the other hello servers (for
example, the one contained in demo/Ice/hello).

To run the demo, first start the server:

     cd ../server
     server

Then start the client:

      client

To test timeouts, use the "Timeout" slider to set an invocation timeout on the
client proxy and the "Delay" slider to force the server to delay its response
long enough to trigger the timeout in the client.
