// Copyright (c) ZeroC, Inc.

plugins {
    id("com.zeroc.gradle.ice-builder.slice") version "1.5.2" apply false
}

subprojects {
    // The application plugin adds support for building Java applications.
    apply(plugin = "application")
    // The Gradle Ice Builder plugin adds support for compiling Slice files.
    apply(plugin = "com.zeroc.gradle.ice-builder.slice")

    repositories {
        // Add the ZeroC Nightly repository to download the Ice artifacts.
        maven("https://download.zeroc.com/nexus/repository/maven-nightly/")
        mavenCentral()
    }
}
