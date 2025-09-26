// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the application plugin to tell gradle this is a runnable Java application.
    id("application")

    // Apply the Slice-tools plugin to enable Slice compilation.
    id("com.zeroc.ice.slice-tools") version "3.8.+"

    // Pull in our local 'convention plugin' to enable linting.
    id("zeroc-linting")
}

dependencies {
    // Add Ice and Glacier2 as implementation dependencies.
    implementation("com.zeroc:ice:3.8.+")
    implementation("com.zeroc:glacier2:3.8.+")
    implementation(project(":util"))
}

sourceSets {
    main {
        // Add the AlarmClock.ice file from the parent slice directory to the main source set.
        slice {
            srcDirs("../slice")
        }
    }
}

application {
    // Specify the main entry point for the application.
    mainClass.set("com.example.glacier2.callback.server.Server")
}
