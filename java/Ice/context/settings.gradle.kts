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
        // This demo uses the latest Ice nightly build published in ZeroC's maven-nightly repository.
        maven("https://central.sonatype.com/repository/maven-snapshots/")
        mavenCentral()
    }
}

rootProject.name = "context"
include("client")
include("server")

includeBuild("../../build-logic")
