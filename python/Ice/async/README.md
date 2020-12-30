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
significant amount of time to complete. For the long running request
the client uses AMI and the server uses AMD to process the request.

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/python-mapping/client-side-slice-to-python-mapping/asynchronous-method-invocation-ami-in-python
[2]: https://doc.zeroc.com/ice/3.7/language-mappings/python-mapping/server-side-slice-to-python-mapping/asynchronous-method-dispatch-amd-in-python
