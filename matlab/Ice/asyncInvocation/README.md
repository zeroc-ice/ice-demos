This demo illustrates the use of [Asynchronous Method Invocation (AMI)][1]
with a server that performs simple calculations and a client that can
call for the calculations without blocking.

Ice for MATLAB currently supports only client-side functionality, therefore
you must use a server from any other language mapping. If you want to get
started quickly, we recommend using the Python server.

After starting the server, run the MATLAB client:

```
>> client
```

[1]: https://doc.zeroc.com/display/Ice37/Asynchronous+Method+Invocation+%28AMI%29+in+MATLAB
