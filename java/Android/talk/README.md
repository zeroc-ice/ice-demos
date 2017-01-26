This directory contains an Android Studio project for the Bluetooth talk
app.

The app allows two Android devices to talk to one another over Bluetooth.
Either user can initiate the connection by choosing "Connect" from the
menu. If the devices aren't already paired, one user should choose the
"Make discoverable" option from the menu while the other chooses "Connect"
and then "Scan for devices".

This app demonstrates several useful techniques that are instructive for
Android developers:

 * Issuing asynchronous Ice invocations to maintain a responsive user
   interface
 * Using bidirectional connections to receive "push notifications" from a
   remote peer
 * Proper separation of UI and application logic

Refer to the [parent directory](../README.md) for build instructions.
