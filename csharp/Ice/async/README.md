This demo illustrates the use of [Asynchronous Method Invocation (AMI)][1]
and [Asynchronous Method Dispatch (AMD)][2].

To run the demo, first start the server:
```
server
```
In a separate window, start the client:
```
client
```

> With .NET 6, use instead:
> ```
> dotnet server.dll
> ```
> and
> ```
> dotnet client.dll
> ```

The shutdown of an server (and its object adapter) waits for outstanding dispatches
to complete. In this demo, we use a CancellationTokenSource to cancel outstanding
dispatches when the application requests a shutdown; this ensures a speedy shutdown.

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/c-sharp-mapping/client-side-slice-to-c-sharp-mapping/asynchronous-method-invocation-ami-in-c-sharp
[2]: https://doc.zeroc.com/ice/3.7/language-mappings/c-sharp-mapping/server-side-slice-to-c-sharp-mapping/asynchronous-method-dispatch-amd-in-c-sharp
