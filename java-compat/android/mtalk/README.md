This directory contains an Android Studio project for the multicast talk
app.

The app allows Android devices to discover one another via multicast and
then establish a secure peer-to-peer connection. Multicast discovery
messages are only sent when users are in the "Connect" dialog. WiFi must
be enabled in all devices.

This app demonstrates several useful techniques that are instructive for
Android developers:

 * Issuing asynchronous Ice invocations to maintain a responsive user
   interface
 * Using bidirectional connections to receive "push notifications" from a
   remote peer
 * Proper separation of UI and application logic

Refer to the [java-compat README.md](../../README.md) for build instructions.
