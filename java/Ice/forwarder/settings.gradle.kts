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

rootProject.name = "forwarder"
include("client")
include("server")
include("forwardingserver")

includeBuild("../../build-logic")
