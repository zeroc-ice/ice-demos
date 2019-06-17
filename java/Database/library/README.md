MySQL JDBC Demo
===============

This demo shows how to implement an Ice server that uses MariaDB or mysql
through a JDBC API and demonstrates the following techniques:

 - Mapping relational data to Ice objects, and in particular the
   conversion between Ice and JDBC types.
 - Using a JDBC connection pool to provide JDBC connections for Ice
   requests.
 - Using an Ice [servant locator][1].
 - Using a [dispatch interceptor][2].
 - Accessing a server with and without [Glacier2][3].

Setting up the demo
-------------------

1. Install mysql or MariaDB as necessary.

2. Create a database named "library" and grant privileges to a user. In
   the commands below, replace USER with the name you have chosen and
   PASSWORD with a suitable password:
    ```
    $ mysql -u root -p
    Enter password:
    Welcome to the MySQL monitor.

    mysql> CREATE DATABASE library;
    Query OK, 1 row affected (0.00 sec)

    mysql> GRANT ALL PRIVILEGES ON library.* TO "USER"@"localhost"
        -> IDENTIFIED BY "PASSWORD";
    Query OK, 0 rows affected (0.00 sec)

    mysql> FLUSH PRIVILEGES;
    Query OK, 0 rows affected (0.01 sec)

    mysql> EXIT
    ```

3. Create the SQL tables using the script createTypes.sql:
    ```
    $ mysql --user=USER --password=PASSWORD library < createTypes.sql
    ```

4. Edit the JDBC properties in config.server to reflect your selected
   user name and password:
    ```
    JDBC.Username=USER
    JDBC.Password=PASSWORD
    ```

Notes
-----

These instructions assume that the demo server runs on the same
host as the database server. If you intend to run the demo server on
a different host than the database server, you will need to revise
the mysql privileges as well as the JDBC URL in `config.server`.

In order to get correct results when using multiple concurrent
clients, you need to use the SERIALIZABLE isolation level. Refer
to the mysql documentation for instructions on how to set the
isolation level:

http://dev.mysql.com/doc/refman/5.0/en/set-transaction.html

Running the demo
----------------

To run the demo, first start the server:

```
java -jar build/libs/server.jar
```

The demo includes a text file named "books" containing a series of
commands that populate the server's database with a collection of
books. Pass this file as an argument the first time you run the
client. In another window:

```
java -jar build/libs/client.jar books
```

Type "help" to get a list of valid commands.

Running the demo with Glacier2
------------------------------

The demo also supports a Glacier2 deployment. You will need to edit
`config.client` and uncomment this configuration property:

```
#Ice.Default.Router=DemoGlacier2/router:ssl -p 4064 -h 127.0.0.1
```

To run the demo using Glacier2, first start the server:

```
java -jar build/libs/server.jar
```

In a separate window, start the Glacier2 router:

```
glacier2router --Ice.Config=config.glacier2
```

In a separate window, start the client:

```
java -jar build/libs/client.jar books
```

Omit the "books" argument if you have already populated the server's
database.

[1]: https://doc.zeroc.com/ice/3.7/server-side-features/servant-locators
[2]: https://doc.zeroc.com/ice/3.7/server-side-features/dispatch-interceptors
[3]: https://doc.zeroc.com/ice/3.7/ice-services/glacier2
