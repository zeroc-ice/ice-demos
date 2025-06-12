# Ice Context

This demo illustrates how to use Ice [request contexts][2].

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
    Context server implemented in a language that fully supports server-side functionality, such as Python, Java, or C#.

4. Run the client application:

    ```shell
    node client.js
    ```

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/javascript-mapping
[2]: https://doc.zeroc.com/ice/3.7/client-side-features/request-contexts
