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

Before running on a physical device, connect it to your Mac, unlock it, and trust the Mac if prompted. Then add your
Apple Account in **Xcode > Settings > Accounts** and select your team in the GreeterApp target's **Signing &
Capabilities** tab — a free Personal Team is sufficient; Apple Developer Program membership is not required. If Xcode
reports that `com.example.ice.GreeterApp` is not available, change the bundle identifier to a unique value.

The device must run iOS 18.5 or later and have **Developer Mode** enabled (Settings > Privacy & Security > Developer
Mode; enabling it restarts the device).

On a physical iOS device, `localhost` refers to the device itself. In the Host field, enter the IP address of the
computer running the Greeter server; the device and this computer must be on the same network.

The first time the app connects, iOS asks for permission to find and connect to devices on your local network. Allow
this request so the app can reach the Greeter server.
