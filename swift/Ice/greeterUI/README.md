# Ice SwiftUI Greeter Demo

This demo illustrates how to create a SwiftUI application that connects to an Ice Greeter server and sends greeting requests.

The demo provides a simple iOS and macOS client with a clean SwiftUI interface similar to the Android Greeter demo. It allows users to:

- Configure the server address and port
- Enter their name
- Send greeting requests to the server
- View the greeting response

## UI Features

- **Server Configuration**: Text fields for server address (defaults to `localhost`) and port (defaults to `4061`)
- **Name Input**: Text field for entering the user's name (auto-populated with system username on macOS)
- **Send Greeting Button**: Initiates the greeting request with loading indicator
- **Response Display**: Shows the greeting response from the server
- **Error Handling**: Displays error alerts for connection or request failures

## Getting Started

First, start a Greeter server implemented in one of the languages with server-side support, such as Python, Java, or C#. For example, you can use the Swift server from the [greeter](../greeter) demo:

```shell
cd ../greeter
swift run Server
```

Then, open this greeterUI project in Xcode and run it on either iOS Simulator, macOS, or a physical iOS device.

> [!NOTE]
> The default server address `localhost` works when running the iOS Simulator on the same machine as the server.
> If your Greeter server is running on a different host, update the server address in the app accordingly.

## Interface Comparison

This SwiftUI demo provides similar functionality to the [Android Greeter demo](../../../../java/Ice/greeter-android) but with a native SwiftUI interface that follows iOS and macOS design patterns.

## Implementation Details

- **Ice Communication**: Uses the same Ice for Swift library and Greeter.ice interface as the command-line [greeter demo](../greeter)
- **Async Operations**: Uses Swift's async/await for non-blocking Ice calls
- **SwiftUI Patterns**: Implements MVVM pattern with `@StateObject` and `@Published` properties
- **Cross-Platform**: Supports both iOS and macOS with platform-specific adaptations
- **Error Handling**: Comprehensive error handling with user-friendly error messages

The demo runs against any Greeter server that implements the `VisitorCenter::Greeter` interface.