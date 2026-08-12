# GreeterApp

This demo shows how to send a request and receive a response using Ice for Swift in a SwiftUI application.

The client is implemented using SwiftUI and can be run on **macOS**, the **iOS Simulator**, or a
**physical iOS device**.

## Getting Started

1. Start the [Greeter server](../Greeter) from the `Greeter` directory.
2. Open the `GreeterApp` project in Xcode.
3. Build and run the application on your platform of choice (macOS, iOS device, or iOS simulator).

> [!IMPORTANT]
> On the first build attempt, you may see the message:
> _"Plugin “CompileSlice” from package “ice” must be enabled before it can be used."_
> Click on the message in Xcode to enable the plugin.

## Running on a physical iOS device

On a physical iOS device, `localhost` refers to the device itself. In the Host field, enter the LAN address of the
computer running the Greeter server (for example, `192.168.1.10`); the device and this computer must be on the same
network.

The first time the app connects, iOS asks for permission to find and connect to devices on your local network. Allow
this request so the app can reach the Greeter server.
