// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the Application plugin to make this project a runnable Java application.
    id("application")
}

dependencies {
    // Include the Ice and IceBox libraries on the runtime classpath.
    runtimeOnly("com.zeroc:ice:3.9.+")
    runtimeOnly("com.zeroc:icebox:3.9.+")

    // Include the service project so its GreeterService class is available at runtime.
    runtimeOnly(project(":service"))
}

application {
    // Set the main entry point of the application to the IceBox server.
    mainClass.set("com.zeroc.IceBox.Server")
}
