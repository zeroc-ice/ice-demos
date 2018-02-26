This demo illustrates how to write a Swing application that invokes
ordinary (twoway) operations, as well as how to make [oneway][1],
[datagram][2], [secure][3], and [batched][4] invocations.

To run the demo, first start the hello server:

```
cd ../hello
java -jar build/libs/server.jar
```

In a separate window, start the Swing client:

```
java -jar build/libs/client.jar
```

[1]: https://doc.zeroc.com/display/Ice37/Oneway+Invocations
[2]: https://doc.zeroc.com/display/Ice37/Datagram+Invocations
[3]: https://doc.zeroc.com/display/Ice37/IceSSL
[4]: https://doc.zeroc.com/display/Ice37/Batched+Invocations
