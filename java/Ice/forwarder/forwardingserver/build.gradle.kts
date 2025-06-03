// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the application plugin to tell gradle this is a runnable Java application.
    id("application")

    // Pull in our local 'convention plugin' to enable linting.
    id("linting-config")
}

dependencies {
    // Add the Ice library as an implementation dependency.
    implementation("com.zeroc:ice:3.8.+")
}

application {
    // Specify the main entry point for the application.
    mainClass.set("com.example.ice.forwarder.forwardingserver.Server")
}
