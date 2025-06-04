# Greeter React

The Greeter React demo illustrates how to write a modern React client application using Ice for JavaScript.

This React application provides:
- An input field to enter a name, with "React" as the default value
- A button to send a greet request
- Upon receiving a response, the application updates the page to display the greeting message
- Error handling for connection issues

Ice for JavaScript has limited server-side support ([see documentation][1]). As a result, you need to start a Greeter
server implemented in a language that fully supports server-side functionality, such as Python, Java, or C#.

## Installation

Before building the React application, install the dependencies:

```shell
npm install
```

## Building the Application

The build process involves two steps:
1. Compile the Slice definitions to generate JavaScript bindings
2. Build the React application

Build everything with:

```shell
npm run build
```

Or build individual components:

```shell
# Generate JavaScript bindings from Slice file
npm run slice

# Start development server
npm run dev
```

## Running the Application

### Development Mode

Start the development server:

```shell
npm run dev
```

Then open your browser and navigate to `http://localhost:5173` (or the URL shown in the console).

### Production Build

After building with `npm run build`, you can serve the production build:

```shell
npm run preview
```

## Prerequisites

Make sure you have a Greeter server running before using the React application. The application expects the server to be available at `localhost:4061` using the WebSocket protocol.

You can start a server from the Ice/config demo:
```shell
cd ../config
<start the server with> --Ice.Default.Protocol=ws
```

Alternatively, you can use other Greeter server implementations, but make sure they are configured to use the WebSocket protocol.

## Usage

1. Start a Greeter server (see Prerequisites above)
2. Start the React application (`npm run dev` or `npm run preview`)
3. Open your browser to the provided URL
4. Enter a name in the input field (defaults to "React")
5. Click "Send Greeting" button
6. The server's greeting response will be displayed below

## Technical Details

This demo uses:
- **React 18** with TypeScript for the user interface
- **Vite** as the build tool and development server
- **@zeroc/ice** package from the ZeroC nightly NPM feed for Ice functionality
- Browser polyfills for Node.js modules required by Ice

The application creates a new Ice communicator for each greeting request and properly cleans it up afterward.

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/javascript-mapping
