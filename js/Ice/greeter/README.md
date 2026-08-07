# Ice Greeter

This demo shows how to send a request and wait for a response using Ice for JavaScript. It includes a Node.js client
application.

## Building the demo

First install npm dependencies:

```shell
npm install
```

Then, to build the client application, run:

```shell
npm run build
```

## Running the demo

Start the client application:

```shell
node client.js
```

> [!NOTE]
> Ice for JavaScript has limited server-side support. By default, the client connects to a Greeter server hosted by
> ZeroC at `hello.zeroc.com`; as a result, you don't need to start a Greeter server to run this demo. If you want to
> use your own Greeter server (implemented in a language with full server-side support, such as C++, C#, Java, Python,
> or Swift), edit the proxy string in `client.ts`: replace `hello.zeroc.com` with `localhost` for a server running on
> the same computer, or with the server's hostname or IP address for a server running on another computer.
