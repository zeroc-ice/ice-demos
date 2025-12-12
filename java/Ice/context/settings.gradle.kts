// Copyright (c) ZeroC, Inc.

pluginManagement {
    repositories {
        mavenLocal()
        gradlePluginPortal()
    }
}

dependencyResolutionManagement {
    repositories {
        mavenCentral()
    }
}

rootProject.name = "context"
include("client")
include("server")

includeBuild("../../build-logic")
