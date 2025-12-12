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

rootProject.name = "bidir"
include("util")
project(":util").projectDir = file("../../util")

include("client")
include("server")

includeBuild("../../build-logic")
