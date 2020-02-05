# Bidir Demo Server App (Universal Windows)

This demo is a graphical server app written in C++/CX. This server works
with the UWP bidir client as well as the Ice/bidir clients provided by
other language mappings, such as C++, C#, Java and Python.

Note: due to UWP restrictions, you cannot run the client (regardless
of the language in which it's implemented) on the same computer as your
UWP server.

This demo shows how to use [bidirectional connections][1] for callbacks.
This is typically used if the server cannot open a connection to the
client to send callbacks, for example, because firewalls block incoming
connections to the client.

## Running the demo

* Build and deploy the demo using `Deploy server` from the `Build` menu.
* Start the "Ice Bidir Demo Server" application from the Windows Start screen
or right click on the project and select `Debug -> Start new instance`.
* On another computer build and start the Ice/bidir client of your choice and
connect using the public IP address of the machine running the server.

[1]: https://doc.zeroc.com/ice/4.0/client-server-features/connection-management/bidirectional-connections
