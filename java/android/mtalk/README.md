This directory contains an Android Studio project for the multicast talk
app.

The app allows Android devices to discover one another via [multicast][1] and
then establish a secure peer-to-peer connection. Multicast discovery
messages are only sent when users are in the "Connect" dialog. WiFi must
be enabled in all devices.

This app demonstrates several useful techniques that are instructive for
Android developers:

 * Issuing [asynchronous Ice invocations][2] to maintain a responsive user
   interface
 * Using [bidirectional connections][3] to receive "push notifications" from
   a remote peer
 * Proper separation of UI and application logic

Refer to the [java README.md](../../README.md) for build instructions.

[2]: https://doc.zeroc.com/ice/3.7/client-side-features/datagram-invocations
[2]: https://doc.zeroc.com/ice/3.7/language-mappings/java-mapping/client-side-slice-to-java-mapping/asynchronous-method-invocation-ami-in-java
[3]: https://doc.zeroc.com/ice/3.7/client-server-features/connection-management/bidirectional-connections
