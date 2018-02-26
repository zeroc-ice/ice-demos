This demo illustrates how to write a MATLAB GUI application that invokes
ordinary (twoway) operations, as well as how to make [oneway][1],
[datagram][2], [secure][3], and [batched][4] invocations.

Ice for MATLAB currently supports only client-side functionality, therefore you
must use a hello server from any other language mapping. If you want to get
started quickly, we recommend using the Python server.

After starting the server, run the MATLAB client:

```
>> client
```

[1]: https://doc.zeroc.com/display/Ice37/Oneway+Invocations
[2]: https://doc.zeroc.com/display/Ice37/Datagram+Invocations
[3]: https://doc.zeroc.com/display/Ice37/IceSSL
[4]: https://doc.zeroc.com/display/Ice37/Batched+Invocations
