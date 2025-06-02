// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the application plugin to tell gradle this is a runnable Java application.
    id("application")

    // Apply the Slice-tools plugin to enable Slice compilation.
    id("com.zeroc.ice.slice-tools") version "3.8.+"
}

dependencies {
    // Add the Ice library as an implementation dependency.
    implementation("com.zeroc:ice:3.8.+")
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
    mainClass.set("com.example.ice.context.client.Client")
}
