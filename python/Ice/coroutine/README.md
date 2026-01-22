This demo shows how to implement a servant using [coroutines][1] and the
await keyword. Python 3.6 or later is required to run this demo.

To run the demo, first start the server:

```
python3 server.py
```

In a second window, start the client:

```
python3 client.py
```

This demo implements an asynchronous dispatch (`FibonnaciI.compute` in
server.py) by making one or more asynchronous invocations, which is a
common pattern.

The coroutine (`FibonnaciI.compute`) sometimes awaits an Ice future
returned by an asynchronous invocation on an Ice proxy.

Note that this demo does not use asyncio and the coroutine returned by
`compute` is NOT asyncio-compatible. This coroutine is handled by Ice's
internal event loop, which is not an asyncio event loop.

An Ice future can only be awaited in such an "Ice dispatch" event loop.
If you want to await an Ice future in an asyncio event loop, you need
to first convert it into an asyncio future with `Ice.wrap_future` (not
shown in this demo).

[1]: https://archive.zeroc.com/ice/3.7/language-mappings/python-mapping/server-side-slice-to-python-mapping/asynchronous-m
