This demo illustrates the use of [Asynchronous Method Invocation (AMI)][1]
and [Asynchronous Method Dispatch (AMD)][2] in combination with asyncio.

It requires Python 3.7 or greater.

To run the demo, first start the server:

```
python3 server.py
```

In a second window, start the client:

```
python3 client.py
```

The demo invocation can either have a short response time or require a
significant amount of time to complete. The client uses AMI for requests to
not block the asyncio event loop and the server uses AMD to process the long
running request.

[1]: https://archive.zeroc.com/ice/3.7/language-mappings/python-mapping/client-side-slice-to-python-mapping/asynchronous-method-invocation-ami-in-python
[2]: https://archive.zeroc.com/ice/3.7/language-mappings/python-mapping/server-side-slice-to-python-mapping/asynchronous-method-dispatch-amd-in-python
