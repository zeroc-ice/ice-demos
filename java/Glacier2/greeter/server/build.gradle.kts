// Copyright (c) ZeroC, Inc.

dependencies {
    repositories {
        mavenLocal()
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-nightly/")
        }
        gradlePluginPortal() // Keep this to allow fetching other plugins
    }
    // Add the Ice library as an implementation dependency.
    implementation("com.zeroc:ice:3.8.0+")
    implementation("com.zeroc:glacier2:3.8.0+")
}

sourceSets {
    main {
        // Add the Greeter.ice file from the parent slice directory the main source set.
        slice {
            srcDirs("../slice")
            // By default a Slice source set includes all Slice files in the srcDirs directories
            // Here we override the default behavior by specifying the list of Slice files to include.
            setIncludes(listOf("Greeter.ice"))
        }
    }
}

application {
    // Specify the main entry point for the application.
    mainClass.set("com.example.ice.greeter.server.Server")
}
