This demo implements the [printer][1] example in the Ice Manual.

To run the demo, first you need to start a Manual printer server from
another language mapping (Java, C++, C#, or Python). If you want to get
started quickly, we recommend using the Python server.

If you're running the client in a browser, you need to start the server with
WS as it's default protocol. This is done by setting the Ice.Default.Protocol
option. If you're running the python server this would look like:

```
python server.py --Ice.Default.Protocol=ws
```

[1]: https://doc.zeroc.com/ice/4.0/hello-world-application/writing-an-ice-application-with-javascript
