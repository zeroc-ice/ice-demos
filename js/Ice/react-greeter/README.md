# Ice React Greeter

This demo shows how to send a request and wait for a response using Ice for JavaScript. It includes a React client
application that runs in the browser.

Follow these steps to build and run the demo:

1\. Install the dependencies:

```shell
npm install
```

2\. Run the server application:

Ice for JavaScript has limited server-side support. As a result, you need to start a Config server implemented in a
language that fully supports server-side functionality, such as Python, Java, or C#.

> [!IMPORTANT]
> Pass `--Ice.Default.Protocol=ws` to the server to instruct it to use the `ws` protocol required for communication
> with the browser client. We use the Ice **Config server** instead of the plain Ice **Greeter server** because it
> allows us to specify `ws` via command-line arguments without modifying the code.

3\. Run the client application in development mode:

```shell
npm run dev
```

    Open a web browser and navigate to http://localhost:5173 (or the URL shown in the terminal).
