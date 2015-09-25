# Bidir Client App for WinRT and Universal Windows Platform (UWP)

This demo is a graphical client app written in C++/CX. This client
works with the Ice/bidir server provided by other language mappings,
such as C++, C#, Java and Python.

This demo shows how to use bidirectional connections for callbacks.
This is typically used if the server cannot open a connection to the
client to send callbacks, for example, because firewalls block
incoming connections to the client.

 > Note: it is possible to implement a "bidir server" in C++/CX
 > for WinRT / UWP as Ice for WinRT / UWP provide server side 
 > support. Such a "bidir server" will be provided soon.

## Running the demo

* Build (if needed) and start the Ice/bidir server of your choice.
* Build and deploy the demo using `Deploy bidir` from the `Build` menu.
* Start the "Ice Bidir Demo" application from the Windows Start screen
or right click on the project and select `Debug -> Start new instance`.

