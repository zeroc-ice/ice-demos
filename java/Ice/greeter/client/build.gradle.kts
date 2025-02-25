// Copyright (c) ZeroC, Inc.

dependencies {
    // Add the Ice library as an implementation dependency.
    implementation("com.zeroc:ice:3.8.0-+")
}


slice {
    java {
        // Configure the default Slice source set to compile the Greeter.ice file from the parent slice directory
        // during the build process.
        create("default") {
            files = listOf(file("../slice/Greeter.ice"))
        }
    }
}

application {
    // Specify the main entry point for the application.
    mainClass.set("com.zeroc.demos.greeter.Client")
}
