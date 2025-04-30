// Copyright (c) ZeroC, Inc.

dependencies {
    repositories {
        mavenLocal()
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-nightly/")
        }
        gradlePluginPortal() // Keep this to allow fetching other plugins
    }
    // Add Ice and Glacier2 as implementation dependencies.
    implementation("com.zeroc:ice:3.8.0-nightly-+")
    implementation("com.zeroc:glacier2:3.8.0-+")
}

sourceSets {
    main {
        // Add the Greeter.ice file from the parent slice directory the main source set.
        slice {
            srcDirs("../slice")
        }
    }
}

application {
    // Specify the main entry point for the application.
    mainClass.set("com.example.glacier2.greeter.client.Client")
}
