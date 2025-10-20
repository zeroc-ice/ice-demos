// Copyright (c) ZeroC, Inc.

pluginManagement {
    repositories {
        mavenLocal()
        gradlePluginPortal()
        // Use the nightly build of the Slice Tools plugin from the ZeroC maven-nightly repository.
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
        // Use the nightly build of Ice from the ZeroC maven-nightly repository.
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-nightly/")
            content {
                includeGroupByRegex("com\\.zeroc.*")
            }
        }
    }
}

rootProject.name = "session"
include("client")
include("server")

includeBuild("../../build-logic")
