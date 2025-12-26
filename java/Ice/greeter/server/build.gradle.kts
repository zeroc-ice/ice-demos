// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the application plugin to tell gradle this is a runnable Java application.
    id("application")

    // Apply the Slice-tools plugin to enable Slice compilation.
    id("com.zeroc.slice-tools") version "3.9.+"

    // Pull in our local 'convention plugin' to enable linting.
    id("zeroc-linting")
}

dependencies {
    // Add the Ice library as an implementation dependency.
    implementation("com.zeroc:ice:3.9.+")
}

sourceSets {
    main {
        // Add the Greeter.ice file from the parent slice directory to the main source set.
        slice {
            srcDirs("../slice")
        }
    }
}

application {
    // Specify the main entry point for the application.
    mainClass.set("com.example.ice.greeter.server.Server")
}
