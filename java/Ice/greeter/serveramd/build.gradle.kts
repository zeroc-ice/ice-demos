// Copyright (c) ZeroC, Inc.

dependencies {
    // Add Ice runtime as an implementation dependency of the project.
    implementation("com.zeroc:ice:3.8.0-nightly-+")
}

slice {
    java {
        // Create a Slice source set to compile GreeterAMD.ice file from the parent slice directory.
        create("amd") {
            files = listOf(file("../slice/GreeterAMD.ice"))
        }
    }
}

application {
    // Define the main class for the application.
    mainClass.set("com.zeroc.demos.greeter.Server")
}
