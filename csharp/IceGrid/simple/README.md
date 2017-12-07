To run the demo, first start the IceGrid service:

```
icegridnode --Ice.Config=config.grid
```

In a separate window:

#### .NET Framework 4.5

```
icegridadmin --Ice.Config=config.grid -e "application add application.xml"
client
```
---

#### .NET Core 2.0

```
icegridadmin --Ice.Config=config.grid -e "application add application-netcore.xml"
dotnet client.dll
```
---

This will deploy the application and start the client. Messages will be displayed in
the IceGrid service window.

You can also use the descriptors in the following files to deploy the
application:

* `application_with_template.xml` and `application-netcore_with_template.xml`:
  These descriptors demonstrate the use of templates for the server definition.
  Templates make it easy to deploy multiple instances of the same server.

* `application_with_replication.xml` and `application_with_replication.xml`: These
  descriptors demonstrate the use of replication to balance the load of the application
  over several servers.

If you have already deployed the application, you can update it to try
a new set of descriptors, for example:

#### .NET Framework 4.5

```
icegridadmin --Ice.Config=config.grid -e "application update application_with_template.xml"
```
---

#### .NET Core 2.0

```
icegridadmin --Ice.Config=config.grid -e "application update application-netcore_with_template.xml"
```
---

When using the descriptors from `application_with_template.xml`, `application-netcore_with_template.xml`,
`application_with_replication.xml` or `application-netcore_with_replication.xml`, you can easily deploy
more servers based on the `SimpleServer` template. For example, you can use the following command to deploy
a new server:

```
icegridadmin --Ice.Config=config.grid -e "server template instantiate Simple node1 SimpleServer index=4"
```
