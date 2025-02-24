// Copyright (c) ZeroC, Inc.

dependencies {
    implementation("com.zeroc:ice:3.8.0-nightly-+")
}

slice {
    java {
        // Create a Slice source set to compile Greeter.ice file from the parent slice directory.
        create("default") {
            files = listOf(file("../slice/Greeter.ice"))
        }
    }
}

application {
    // Define the main class for the application.
    mainClass.set("com.zeroc.demos.greeter.Server")
}
