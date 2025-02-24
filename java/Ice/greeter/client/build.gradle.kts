// Copyright (c) ZeroC, Inc.

dependencies {
    // Add Ice runtime as an implementation dependency of the project.
    implementation("com.zeroc:ice:3.8.0-+")
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
    mainClass.set("com.zeroc.demos.greeter.Client")
}
