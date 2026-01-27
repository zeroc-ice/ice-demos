This demo illustrates the basics of using [IceGrid], including the use of [templates] and [object adapter replication].

To run the demo, first start the IceGrid service:

```shell
icegridnode --Ice.Config=config.grid
```

In a separate window:

```shell
icegridadmin --Ice.Config=config.grid -e "application add application.xml"
dotnet client.dll
```

This will deploy the application and start the client. Messages will be displayed in the IceGrid service window.

You can also use the descriptors in the following files to deploy the application:

* `application_with_template.xml`: These descriptors demonstrate the use of templates for the server definition.
  Templates make it easy to deploy multiple instances of the same server.

* `application_with_replication.xml`: These descriptors demonstrate the use of replication to balance the load of the
  application over several servers.

If you have already deployed the application, you can update it to try a new set of descriptors, for example:

```shell
icegridadmin --Ice.Config=config.grid -e "application update application_with_template.xml"
```

When using the descriptors from `application_with_template.xml`, or `application_with_replication.xml`, you can easily
deploy more servers based on the `SimpleServer` template. For example, you can use the following command to deploy a
new server:

```shell
icegridadmin --Ice.Config=config.grid -e "server template instantiate Simple node1 SimpleServer index=4"
```

[IceGrid]: https://archive.zeroc.com/ice/3.7/ice-services/icegrid
[templates]: https://archive.zeroc.com/ice/3.7/ice-services/icegrid/icegrid-templates
[object adapter replication]: https://archive.zeroc.com/ice/3.7/ice-services/icegrid/object-adapter-replication
