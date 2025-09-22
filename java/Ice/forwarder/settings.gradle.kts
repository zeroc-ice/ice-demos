// Copyright (c) ZeroC, Inc.

pluginManagement {
    repositories {
        mavenLocal()
        gradlePluginPortal()
        // This demo uses the nightly build of the Slice Tools plugin, published to the ZeroC Maven Nightly repository.
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-nightly/")
            content {
                includeGroupByRegex("com\\.zeroc.*")
            }
        }
    }
}

dependencyResolutionManagement {
    repositories {
        mavenCentral()
        // This demo uses the nightly build of Ice, published to the ZeroC Maven Nightly repository.
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-nightly/")
            content {
                includeGroupByRegex("com\\.zeroc.*")
            }
        }
    }
}

rootProject.name = "forwarder"
include("client")
include("server")
include("forwardingserver")

includeBuild("../../build-logic")
