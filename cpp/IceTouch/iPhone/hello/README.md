This demo illustrates how to integrate Ice with a simple graphical
application.

The demo shows how to make:

  * Twoway invocations. This is the default mode for any remote
    invocation. A twoway invocation is not complete until a reply has
    been received from the server.

  * Oneway invocations. A oneway invocation is complete as soon as the
    client has successfully sent the invocation. Oneway invocations
    may not return values or throw user exceptions.

  * Oneway batched invocations. See below for details on batching.

  * Datagram invocations. Datagram invocations use the udp transport,
    and as such have very minimal quality of service guarantees.
    Datagram invocations are always oneway.

  * Datagram batched invocations. See below for details on batching.

Batching, which is only possible when using oneway or datagram
invocations, combines several Ice messages into a single physical
protocol message. Batching can result in higher throughput and smaller
physical messages due to less protocol overhead. Ice does not actually
send a batched message until it is flushed.

If you enable secure invocations, oneway and twoway operations are
sent using the SSL transport. It is not possible to secure datagram
invocations.

All invocations use asynchronous method invocations (AMI) to avoid
blocking the GUI thread and ensure a responsive user experience.

This demo runs against any hello server, such as the
[C++ server](../../../Ice/hello).

### Timeouts

To test timeouts you can use the timeout slider to set a timeout in
the client proxy and the delay slider to set a delayed response in
the server to cause a timeout.

You will notice that two "Hello World!" messages will be printed by
the server in this case. This is because the sayHello method is marked
as idempotent in the Slice, meaning that Ice does not need to follow
the at-most-once retry semantics.

See the manual for more information about retry behavior.

### IceDiscovery

Instead of configuring the IP address for the server you can use IceDiscovery to
discover the server.

To enable IceDiscovery, touch the "Use IceDiscovery" button to switch it on. You
must run the hello server from the IceDiscovery/hello demo (such as the [C++
server](../../../IceDiscovery/hello)) and ensure that the server is listening on
an accessible network interface.

See the README file of the IceDiscovery hello demo for more information on how
to start the server.