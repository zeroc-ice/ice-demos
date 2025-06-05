// Copyright (c) ZeroC, Inc.

pluginManagement {
    repositories {
        mavenLocal()
        maven("https://download.zeroc.com/nexus/repository/maven-nightly/")
        google() // For Android Gradle Plugin
        gradlePluginPortal() // Keep this to allow fetching other plugins
    }
}

dependencyResolutionManagement {
    repositories {
        // This demo uses the latest Ice nightly build published in ZeroC's maven-nightly repository.
        maven("https://download.zeroc.com/nexus/repository/maven-nightly/")
        google() // For Android dependencies
        mavenCentral()
    }
}

rootProject.name = "greeter"
include("client")
include("server")
include("serveramd")
include("android")
