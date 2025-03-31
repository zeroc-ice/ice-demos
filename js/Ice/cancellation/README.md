# Ice Cancellation

The Cancellation demo shows how to cancel an invocation using the AsyncResult cancel method. It also shows a related
feature: invocation timeouts.

Follow these steps to build and run the demo:

1. Install the dependencies:

    ```shell
    npm install
    ```

2. Build the client application:

    ```shell
    npm run build
    ```

3. Run the server application:

    Ice for JavaScript has limited server-side support ([see documentation][1]). As a result, you need to start a
    Glacier2 Callback server implemented in a language that fully supports server-side functionality, such as Python,
    Java, or C#.

    ```shell
    cd Server
    dotnet run
    ```

4. Run the client application:

    ```shell
    node client.js --Ice.Trace.Network
    ```

    > [!NOTE]
    > The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that the
    > `InvocationCanceledException` and `InvocationTimeoutException` do not close the connection.

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/javascript-mapping
