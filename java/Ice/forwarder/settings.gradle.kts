// Copyright (c) ZeroC, Inc.

pluginManagement {
    repositories {
        mavenLocal()
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-nightly/")
        }
        gradlePluginPortal() // Keep this to allow fetching other plugins
    }
}

rootProject.name = "forwarder"
include("client")
include("server")
include("forwardingserver")
