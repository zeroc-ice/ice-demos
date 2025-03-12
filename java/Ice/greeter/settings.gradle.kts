// Copyright (c) ZeroC, Inc.

pluginManagement {
    repositories {
        mavenLocal()
        gradlePluginPortal() // Keep this to allow fetching other plugins
    }
}

rootProject.name = "greeter"
include("client")
include("server")
include("serveramd")
