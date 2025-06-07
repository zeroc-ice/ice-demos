// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the Kotlin-DSL plugin development plugin.
    `kotlin-dsl`
}

repositories {
    mavenCentral()
    gradlePluginPortal()
}

dependencies {
    // This is the only way to pull in a specific version of the 'openrewrite' plugin. Convention plugins like this one
    // can't specify plugin versions in the `plugins {}` DSL block (where you normally would).
    implementation("org.openrewrite.rewrite:org.openrewrite.rewrite.gradle.plugin:7.7.0")
}
