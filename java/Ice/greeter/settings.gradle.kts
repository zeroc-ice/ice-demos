// Copyright (c) ZeroC, Inc.

pluginManagement {
    includeBuild("../../convention")
    repositories {
        mavenLocal()
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-nightly/")
        }
        gradlePluginPortal() // Keep this to allow fetching other plugins
    }
}

rootProject.name = "greeter"
include("client")
include("server")
include("serveramd")
include(":plugin")
