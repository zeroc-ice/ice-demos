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

## Building the App

### Prerequisites

- Xcode 15.0 or later
- iOS 17.0+ / macOS 14.0+ deployment targets
- Ice for Swift library (automatically managed by Swift Package Manager)

### For iOS

1. **Create a new iOS App project in Xcode:**
   - Open Xcode and select "Create a new Xcode project"
   - Choose "iOS" → "App"
   - Set Product Name to "Greeter"
   - Select "SwiftUI" for Interface
   - Choose a Bundle Identifier (e.g., `com.yourname.greeter`)
   - Select Swift as the language

2. **Add the Ice for Swift package dependency:**
   - In Xcode, go to File → Add Package Dependencies
   - Enter the URL: `https://download.zeroc.com/nexus/repository/nightly/ice-swift.git`
   - Select "Up to Next Major Version" and click "Add Package"
   - Select the "Ice" library and click "Add Package"

3. **Replace the default files with the demo files:**
   - Delete the default `ContentView.swift` and `GreeterApp.swift` files from the project
   - Add all files from this directory to your Xcode project:
     - `GreeterApp.swift`
     - `GreeterView.swift`
     - `GreeterClient.swift`
     - `Greeter.ice`
     - `Info.plist` (replace the generated one)
     - `Images.xcassets` (replace the generated one)

4. **Configure the project:**
   - In the project settings, set the minimum iOS deployment target to 17.0
   - Ensure the Bundle Identifier matches what you set during project creation

5. **Build and run:**
   - Select an iOS Simulator or connected iOS device
   - Press Cmd+R to build and run the app

### For macOS

1. **Create a new macOS App project in Xcode:**
   - Open Xcode and select "Create a new Xcode project"
   - Choose "macOS" → "App"
   - Set Product Name to "Greeter"
   - Select "SwiftUI" for Interface
   - Choose a Bundle Identifier (e.g., `com.yourname.greeter-macos`)
   - Select Swift as the language

2. **Add the Ice for Swift package dependency:**
   - Follow the same steps as for iOS (steps 2 above)

3. **Replace the default files with the demo files:**
   - Follow the same steps as for iOS (step 3 above)

4. **Configure the project:**
   - In the project settings, set the minimum macOS deployment target to 14.0

5. **Build and run:**
   - Select "My Mac" as the destination
   - Press Cmd+R to build and run the app

### For Universal (iOS + macOS) App

1. **Create a multiplatform app:**
   - Open Xcode and select "Create a new Xcode project"
   - Choose "Multiplatform" → "App"
   - Follow the same configuration steps as above
   - The app will automatically build for both iOS and macOS targets

> [!NOTE]
> The default server address `localhost` works when running the iOS Simulator or macOS app on the same machine as the server.
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
