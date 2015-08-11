This demo demonstrates the use of a Glacier2 session helpers and swing
to create a graphical client for the simple chat server.

First follow the instructions in the C++ Glacier2 simpleChat demo
README to start the server and the Glacier2 router.

Note: On OS X it will be necessary to add the following line to
      config.glacier2 due to a bug in OS X's Secure Transport
      implementation of TLS 1.2.

      IceSSL.ProtocolVersionMax=tls1_1

In a separate window, start the swing client:

$ java -jar build/libs/client.jar
