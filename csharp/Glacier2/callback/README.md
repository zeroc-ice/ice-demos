This example demonstrates the use of several Glacier2 features, including
[callbacks][1] and [request contexts][2].

To run the demo, first start the server:

| .NET Framework 4.5 | .NET Core 2.0        |
| ------------------ | -------------------- |
| `server`           | `dotnet server.dll`  |

In a separate window, start the Glacier2 router:

```
glacier2router --Ice.Config=config.glacier2
```

In a separate window, start the client:

| .NET Framework 4.5 | .NET Core 2.0       |
| ------------------ | ------------------- |
| `client`           | `dotnet client.dll` |

If you plan to run this demo using clients on different hosts than
the router, you must first modify the configuration. You need to
change the `Glacier2.Client.Endpoints` property in `config.glacier2` and
the `Ice.Default.Router` and `Callback.Client.Router` properties in
`config.client`. In all cases you must replace the `-h 127.0.0.1`
parameter with the actual external address of the machine on which
glacier2router is running.

[1]: https://doc.zeroc.com/display/Ice37/Callbacks+through+Glacier2
[2]: https://doc.zeroc.com/display/Ice37/How+Glacier2+uses+Request+Contexts
