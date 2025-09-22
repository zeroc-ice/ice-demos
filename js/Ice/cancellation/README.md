# Ice Cancellation

The Cancellation demo shows how to cancel an invocation using the AsyncResult cancel method. It also shows a related
feature: invocation timeouts.

## Building the demo

First instal npm dependencies:

```shell
npm install
```

Then, to build the client application, run:

```shell
npm run build
```

## Running the demo

Run the server application.

Ice for JavaScript has limited server-side support. As a result, you need to start an Ice Cancellation server
implemented in a language that fully supports server-side functionality, such as Python, Java, or C#.

In a separate terminal, start the client application:

```shell
node client.js --Ice.Trace.Network
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that the
> `InvocationCanceledException` and `InvocationTimeoutException` do not close the connection.
