// Copyright (c) ZeroC, Inc.

pluginManagement {
    repositories {
        mavenLocal()
        gradlePluginPortal()
        // Use the nightly build of the Slice Tools plugin from the Maven Central snapshots repository.
        maven {
            url = uri("https://central.sonatype.com/repository/maven-snapshots")
            content {
                includeGroupByRegex("com\\.zeroc.*")
            }
        }
    }
}

dependencyResolutionManagement {
    repositories {
        mavenCentral()
        // Use the nightly build of Ice from the Maven Central snapshots repository.
        maven {
            url = uri("https://central.sonatype.com/repository/maven-snapshots")
            content {
                includeGroupByRegex("com\\.zeroc.*")
            }
        }
    }
}

rootProject.name = "inheritance"
include("client")
include("server")

includeBuild("../../build-logic")
