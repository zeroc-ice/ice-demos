# Ice Context

This demo illustrates how to use Ice request contexts.

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

Ice for JavaScript has limited server-side support. As a result, you need to start a Context server
implemented in a language that fully supports server-side functionality, such as Python, Java, or C#.

In a separate terminal, start the client application:

```shell
node client.js
```
