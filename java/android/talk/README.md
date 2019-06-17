This directory contains an Android Studio project for the [Bluetooth][1]
talk app.

The app allows two Android devices to talk to one another over Bluetooth.
Either user can initiate the connection by choosing "Connect" from the
menu. If the devices aren't already paired, one user should choose the
"Make discoverable" option from the menu while the other chooses "Connect"
and then "Scan for devices".

This app demonstrates several useful techniques that are instructive for
Android developers:

 * Issuing [asynchronous Ice invocations][2] to maintain a responsive user
   interface
 * Using [bidirectional connections][3] to receive "push notifications" from
   a remote peer
 * Proper separation of UI and application logic

Refer to the [java README.md](../../README.md) for build instructions.

[1]: https://doc.zeroc.com/display/Ice37/IceBT
[2]: https://doc.zeroc.com/display/Ice37/Asynchronous+Method+Invocation+%28AMI%29+in+Java
[3]: https://doc.zeroc.com/ice/3.7/client-server-features/connection-management/bidirectional-connections
