# IceGrid Simple

This demo illustrates the basics of using [IceGrid][1], including the use of
[templates][2] and [object adapter replication][3].

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first open a new terminal window for the IceGrid service.

Add the server build directory to the PATH environment variable:

**Linux/macOS:**

```shell
export PATH=$PWD/build:$PATH
```

**Windows:**

```shell
set PATH=%CD%\\build\\Release;%PATH%
```

Now start the IceGrid service:

```shell
icegridnode --Ice.Config=config.grid
```

In a separate window deploy the application as described in the file `application.xml`:

```shell
icegridadmin --Ice.Config=config.admin -e "application add application.xml"
```

Next, start the client

```shell
client
```

Messages will be displayed in the IceGrid service window.

You can also use the descriptors in the following files to deploy
the application:

- `application_with_template.xml`: These descriptors demonstrate the use
  of templates for the server definition. Templates make it easy to
  deploy multiple instances of the same server.

- `application_with_replication.xml`: These descriptors demonstrate the
  use of replication to balance the load of the application over
  several servers.

If you have already deployed the application, you can update it to try
a new set of descriptors, for example:

```shell
icegridadmin --Ice.Config=config.admin -e "application update application_with_template.xml"
```

When using the descriptors from `application_with_template.xml` or
`application_with_replication.xml`, you can easily deploy more servers
based on the `SimpleServer` template. For example, you can use the
following command to deploy a new server:

```shell
icegridadmin --Ice.Config=config.admin -e "server template instantiate Simple node1 SimpleServer index=4"
```

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icegrid
[2]: https://doc.zeroc.com/ice/3.7/ice-services/icegrid/icegrid-templates
[3]: https://doc.zeroc.com/ice/3.7/ice-services/icegrid/object-adapter-replication
