# Ice Greeter Android Demo

This directory contains an Android version of the Ice Greeter demo client.

## Requirements

- Android SDK (API level 21 or higher)
- Java 8 or higher
- Gradle 8.0 or higher
- Android Gradle Plugin 7.4+ or 8.0+

## Features

The Android application provides:
- Input field for server IP address (default: 10.0.2.2 for Android emulator)
- Input field for user name (default: "Android")
- Button to send greeting request to the Ice server
- Display area for server response or error messages
- Non-blocking async communication using `greetAsync`
- Proper Ice communicator lifecycle management

## Building

1. Ensure you have Android SDK installed and configured
2. From the parent greeter directory, run:
   ```bash
   ./gradlew android:build
   ```

## Running

1. Start the Ice greeter server (from the server or serveramd directory)
2. Install the APK on an Android device or emulator
3. Enter the server IP address (use 10.0.2.2 for emulator to reach host machine)
4. Enter your name and tap "Send Greeting"

## Project Structure

- `MainActivity.java` - Main UI activity with greeting functionality
- `GreeterApplication.java` - Application class managing Ice communicator lifecycle
- `res/layout/activity_main.xml` - UI layout definition
- `res/values/strings.xml` - String resources
- `AndroidManifest.xml` - Android application manifest

## Technical Details

- Uses Ice Slice-generated code from `../slice/Greeter.ice`
- Implements async communication to keep UI responsive
- Handles connection errors gracefully with user feedback
- Follows Android best practices for network operations