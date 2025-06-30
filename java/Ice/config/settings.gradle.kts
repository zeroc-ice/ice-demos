// Copyright (c) ZeroC, Inc.

pluginManagement {
    repositories {
        mavenLocal()
        maven("https://central.sonatype.com/repository/maven-snapshots/")
        gradlePluginPortal() // Keep this to allow fetching other plugins
    }
}

dependencyResolutionManagement {
    repositories {
        // This demo uses the latest Ice nightly build published to the maven central snapshots repository.
        maven("https://central.sonatype.com/repository/maven-snapshots/")
        mavenCentral()
    }
}

rootProject.name = "config"
include("client")
include("server")

includeBuild("../../build-logic")
