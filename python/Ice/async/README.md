This demo illustrates the use of Asynchronous Method Invocation (AMI)
and Asynchronous Method Dispatch (AMD).

To run the demo, first start the server:

```
python server.py
```

In a second window, start the client:

```
python client.py
```

The demo invocation can either have a short response time or require a
significant amount of time to complete. For the long running request
the client uses AMI and the server uses AMD plus a worker thread to
process the request. While a long request is processing, short
requests are still able to be processed and more long requests can be
queued for processing by the worker thread.
