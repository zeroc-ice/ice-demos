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

rootProject.name = "secure"
include("client")
include("server")

includeBuild("../../build-logic")
