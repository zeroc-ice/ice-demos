# Ice helloUI

This demo illustrates how to write a SwiftUI application that invokes ordinary (twoway) operations, as well as how to
make [oneway], [datagram], [secure], and [batched] invocations.

## Building the Demo

Open the Xcode project:

```shell
open helloUI.xcodeproj
```

Build and run on an iOS simulator or device. The first build will fetch the Ice and PromiseKit Swift packages.

## Running the Demo

The demo shows how to make:

- Twoway invocations. This is the default mode for any remote invocation. A twoway invocation is not complete until a
  reply has been received from the server.

- Oneway invocations. A [oneway] invocation is complete as soon as the client has successfully sent the invocation.
  Oneway invocations may not return values or throw user exceptions.

- Oneway batched invocations. See below for details on batching.

- Datagram invocations. [Datagram][datagram] invocations use the udp transport, and as such have very minimal quality of
  service guarantees. Datagram invocations are always oneway.

- Datagram batched invocations. See below for details on batching.

[Batching][batched], which is only possible when using oneway or datagram invocations, combines several Ice messages
into a single physical protocol message. Batching can result in higher throughput and smaller physical messages due to
less protocol overhead. Ice does not actually send a batched message until it is flushed.

If you enable secure invocations, oneway and twoway operations are sent using the [SSL transport][secure]. It is not
possible to secure datagram invocations.

All invocations use asynchronous method invocations (AMI) or batch invocations to avoid blocking the GUI thread and
ensure a responsive user experience.

This demo runs against any hello server, such as the [Swift server].

### Timeouts

To test [timeouts] you can use the timeout slider to set an invocation timeout in the client proxy and the delay slider
to set a delayed response in the server to cause a timeout.

[Swift server]: ../hello
[oneway]: https://doc.zeroc.com/ice/3.7/client-side-features/oneway-invocations
[datagram]: https://doc.zeroc.com/ice/3.7/client-side-features/datagram-invocations
[batched]: https://doc.zeroc.com/ice/3.7/client-side-features/batched-invocations
[secure]: https://doc.zeroc.com/ice/3.7/ice-plugins/icessl
[timeouts]: https://doc.zeroc.com/ice/3.7/client-side-features/invocation-timeouts
