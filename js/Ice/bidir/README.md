# Ice Bidir

The Bidir demo illustrates how to send requests "the other way around", from a server to a client, by reusing the
connection established by the client to the server.

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

    Ice for JavaScript has limited server-side support. As a result, you need to start a Bidir server implemented in a
    language that fully supports server-side functionality, such as Python, Java, or C#.

4. Run the client application:

    ```shell
    node client.js
    ```
