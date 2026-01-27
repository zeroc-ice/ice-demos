This demo illustrates the use of [Asynchronous Method Invocation (AMI)] and [Asynchronous Method Dispatch (AMD)].

To run the demo, first start the server:

```shell
dotnet server.dll
```

In a separate window, start the client:

```shell
dotnet client.dll
```

The shutdown of a server (and its object adapter) waits for outstanding dispatches to complete. In this demo, we use a
CancellationTokenSource to cancel outstanding dispatches when the application requests a shutdown; this ensures a
speedy shutdown.

[Asynchronous Method Invocation (AMI)]: https://archive.zeroc.com/ice/3.7/language-mappings/c-sharp-mapping/client-side-slice-to-c-sharp-mapping/asynchronous-method-invocation-ami-in-c-sharp
[Asynchronous Method Dispatch (AMD)]: https://archive.zeroc.com/ice/3.7/language-mappings/c-sharp-mapping/server-side-slice-to-c-sharp-mapping/asynchronous-method-dispatch-amd-in-c-sharp
