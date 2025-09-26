// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the java-library plugin to tell gradle this is a Java library.
    id("java-library")

    // Apply the Slice-tools plugin to enable Slice compilation.
    id("com.zeroc.slice-tools") version "3.8.+"

    // Pull in our local 'convention plugin' to enable linting.
    id("zeroc-linting")
}

dependencies {
    // Add the Ice and IceBox libraries as implementation dependencies.
    implementation("com.zeroc:ice:3.8.+")
    implementation("com.zeroc:icebox:3.8.+")
}

sourceSets {
    main {
        // Add the Greeter.ice file from the parent slice directory to the main source set.
        slice {
            srcDirs("../slice")
        }
    }
}
