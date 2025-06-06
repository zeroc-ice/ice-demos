# Ice Greeter

This demo shows how to send a request and wait for a response using Ice for JavaScript. It includes a Node.js client
application.

Ice for JavaScript has limited server-side support ([see documentation][1]). As a result, you first need to start a
Greeter server implemented in a language that fully supports server-side functionality, such as Python, Java, or C#.

Then, in a separate terminal:

- Navigate to the demo directory:

  ```shell
  cd js/Ice/greeter
  ```

- Install JavaScript dependencies:

  ```shell
  npm install
  ```

- Build the client application:

  ```shell
  npm run build
  ```

- Run the client application:

  ```shell
  node client.js
  ```

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/javascript-mapping
