This demo illustrates how to implement and use string converters
with Ice. In this demo, the clients uses ISO-Latin-1 as its
character set, while the server uses UTF-8.

The demo sends and receives the greeting "Bonne journée" which
in Latin-1 encoding is "Bonne journ\351e" and in UTF-8 encoding
is "Bonne journ\303\251e".

The demo server prints the strings as they are received to show
how, without conversion, they are not in the format expected by
the application.

There are two clients Client1.py uses an string converter and,
Client2.py doesn't use and string converter.

To run the demo, first start the server:

$ python Server.py

In a separate window, start Client1.py:

$ python Client1.py

In a separate window, start Client2.py:

$ python Client2.py
