This demo is a Cocoa client for the Java library demo located at
demoj/Database/library in the Ice distribution. This application
provides a graphical interface to the library server. It permits you
to:

  - Create and delete books
  - Query for books by isbn, author or title
  - Rent and return books

ZeroC hosts a server for this demo on demo2.zeroc.com. The iPhone
client connects to this host by default. The deployment on
demo2.zeroc.com accepts both secure and non-secure connections using
Glacier2 or direct connections to the library session manager.
