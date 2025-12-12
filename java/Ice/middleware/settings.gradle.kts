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

rootProject.name = "middleware"
include("client")
include("server")

includeBuild("../../build-logic")
