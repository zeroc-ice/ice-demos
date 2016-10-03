# Hello Demo Server App (Universal Windows)

This demo is a graphical server app written in C++/CX. This server
works with the UWP hello client as well as the Ice/hello clients provided
by other language mappings, such as C++, C#, Java and Python.

Note that due to UWP restrictions the client must be run on a different
machine from the server.

## Running the demo

* Build and deploy the demo using `Deploy server` from the `Build` menu.
* Start the "Ice Hello Demo Server" application from the Windows Start screen
or right click on the project and select `Debug -> Start new instance`.
* On another machine build and start the Ice/hello client of your choice and
connect using the public IP address of the machine running the server.

Note that UWP servers don't accept SSL connections.

