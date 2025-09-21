// Copyright (c) ZeroC, Inc.

pluginManagement {
    repositories {
        mavenLocal()
        maven("https://download.zeroc.com/nexus/repository/maven-nightly/")
        gradlePluginPortal() // Keep this to allow fetching other plugins
    }
}

dependencyResolutionManagement {
    repositories {
        // This demo uses the latest Ice nightly build published to the maven central snapshots repository.
        maven("https://download.zeroc.com/nexus/repository/maven-nightly/")
        mavenCentral()
    }
}

rootProject.name = "bidir"
include("util")
project(":util").projectDir = file("../../util")

include("client")
include("server")

includeBuild("../../build-logic")
