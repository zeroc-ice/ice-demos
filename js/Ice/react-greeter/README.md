# Ice React Greeter

This demo shows how to send a request and wait for a response using Ice for JavaScript. It includes a React client
application that runs in the browser.

## Building the demo

First instal npm dependencies:

```shell
npm install
```

Then, to build the sensor application, run:

```shell
npm run build
```

## Running the demo

Run the server application:

Ice for JavaScript has limited server-side support. As a result, you need to start a Config server implemented in a
language that fully supports server-side functionality, such as Python, Java, or C#.

> [!IMPORTANT]
> Pass `--Ice.Default.Protocol=ws` to the server to instruct it to use the `ws` protocol required for communication
> with the browser client. We use the Ice **Config server** instead of the plain Ice **Greeter server** because it
> allows us to specify `ws` via command-line arguments without modifying the code.

In a separate terminal, start the client application:

```shell
npm run dev
```

Open a web browser and navigate to http://localhost:5173 (or the URL shown in the terminal).
