This demo is an Ice Touch client for the [Java library demo][1]. It provides
a command-line interface to the library server and permits you to:

  - Create and delete books
  - Query for books by ISBN, author or title
  - Rent and return books

ZeroC hosts a server for this demo on demo2.zeroc.com, which is the
default host used by this client. The deployment on demo2.zeroc.com
accepts both secure and non-secure connections using Glacier2. Direct
connections to the library session manager are not permitted on
demo2.zeroc.com.

[1]: ../../../java/Database/library
