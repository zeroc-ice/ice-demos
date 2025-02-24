// Copyright (c) ZeroC, Inc.

plugins {
    java
    application
    id("com.zeroc.gradle.ice-builder.slice") version "1.5.1"
}

dependencies {
    implementation("com.zeroc:ice:3.8.0-nightly-+")
}

slice {
    java {
        files = listOf(file("../slice/Greeter.ice"))
    }
}

application {
    mainClass.set("com.zeroc.demos.greeter.Server")
}
