# Ice Filesystem

This demo implements the filesystem application shown at the end of JavaScript mapping chapter in the Ice manual.

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
    Filesystem server implemented in a language that fully supports server-side functionality, such as Python, Java, or C#.

4. Run the client application:

    ```shell
    node client.js
    ```

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/javascript-mapping
