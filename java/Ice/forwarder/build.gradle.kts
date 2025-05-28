// Copyright (c) ZeroC, Inc.

plugins {
    // Register the Slice tools plugin without applying it here.
    // The plugin will be applied to all subprojects in the `subprojects` block below.
    id("com.zeroc.ice.slice-tools") version "3.8.0-+" apply false
}

subprojects {
    // Apply the application plugin to enable Java application support in subprojects.
    apply(plugin = "application")

    // Apply the Slice tools plugin to enable Slice compilation in all subprojects.
    apply(plugin = "com.zeroc.ice.slice-tools")

    repositories {
        // This demo uses the latest Ice nightly build published in ZeroC's maven-nightly repository.
        maven("https://download.zeroc.com/nexus/repository/maven-nightly/")
        mavenCentral()
    }

    tasks.withType<JavaCompile>().configureEach {
        options.compilerArgs.add("-Xlint:all")
    }
}
