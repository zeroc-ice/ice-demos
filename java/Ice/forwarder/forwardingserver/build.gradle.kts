// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the application plugin to tell gradle this is a runnable Java application.
    id("application")
}

dependencies {
    // Add the Ice library as an implementation dependency.
    implementation("com.zeroc:ice:3.8.+")
}

application {
    // Specify the main entry point for the application.
    mainClass.set("com.example.ice.forwarder.forwardingserver.Server")
    applicationDefaultJvmArgs = listOf("-ea") // Enables assertions
}
