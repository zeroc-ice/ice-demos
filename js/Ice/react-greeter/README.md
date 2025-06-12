# Ice React Greeter

This demo shows how to send a request and wait for a response using Ice for JavaScript. It includes a React client
application that runs in the browser.

Ice for JavaScript has limited server-side support ([see documentation][1]). As a result, you first need to start a
Config server implemented in a language that fully supports server-side functionality, such as Python, Java, or C#.

> !IMPORTANT
> Pass `--Ice.Default.Protocol=ws` to the server to instruct it to use the `ws` protocol required for communication with
> the browser client. We use the Ice **Config server** instead of the plain Ice **Greeter server** because it allows us
> to specify `ws` via command-line arguments without modifying the code.

Then, in a separate terminal:

- Navigate to the demo directory:

  ```shell
  cd js/Ice/react-greeter
  ```

- Install JavaScript dependencies:

  ```shell
  npm install
  ```

## Development Build

To run the client in development mode:

- Start the development server:

  ```shell
  npm run dev
   ```

- Open a web browser and navigate to http://localhost:5173 (or the URL shown in the terminal).

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/javascript-mapping
