This demo illustrates how to integrate Ice with a simple graphical
application.

The demo shows how to make:

  - Twoway invocations. This is the default mode for any remote
    invocation. A twoway invocation is not complete until a reply has
    been received from the server.

  - Oneway invocations. A oneway invocation is complete as soon as the
    client has successfully sent the invocation. Oneway invocations
    may not return values or throw user exceptions.

  - Oneway batched invocations. See below for details on batching.

  - Datagram invocations. Datagram invocations use the udp transport,
    and as such have very minimal quality of service guarantees.
    Datagram invocations are always oneway.

  - Datagram batched invocations. See below for details on batching.

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

This demo runs against any hello server, such as the [Objective-C
server](../../../Ice/hello).

### Timeouts

To test timeouts, use the "Timeout" slider to set an invocation timeout on the
client proxy and the "Delay" slider to force the server to delay its response
long enough to trigger the timeout in the client.

### IceDiscovery

Instead of configuring the IP address for the server you can use IceDiscovery to
discover the server.

To enable IceDiscovery, touch the "Use IceDiscovery" button to switch it on. You
must run the hello server from the IceDiscovery/hello demo (such as the
[Objective-C server](../../../IceDiscovery/hello)) and ensure that the server is
listening on an accessible network interface.

See the README file of the IceDiscovery hello demo for more information on how
to start the server.
