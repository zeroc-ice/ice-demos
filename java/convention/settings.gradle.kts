// Copyright (c) ZeroC, Inc.

dependencyResolutionManagement {
    repositories {
        mavenLocal()
        mavenCentral()
        maven {
            url = uri("https://repo1.maven.org/maven2/")
        }
        gradlePluginPortal() // Keep this to allow fetching other plugins
    }
    versionCatalogs {
        create("libraries") {
            from(files("libraries.versions.toml"))
        }
    }
}

rootProject.name = "convention"
include(":plugin")
