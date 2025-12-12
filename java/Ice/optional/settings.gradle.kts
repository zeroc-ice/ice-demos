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

rootProject.name = "optional"
include("client1")
include("client2")
include("server1")
include("server2")

includeBuild("../../build-logic")
