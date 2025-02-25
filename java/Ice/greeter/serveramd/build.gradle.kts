// Copyright (c) ZeroC, Inc.

dependencies {
    // Add the Ice library as an implementation dependency.
    implementation("com.zeroc:ice:3.8.0-nightly-+")
}

slice {
    java {
        // Configure the default Slice source set to compile the GreeterAMD.ice file from the parent slice directory
        // during the build process.
        create("amd") {
            files = listOf(file("../slice/GreeterAMD.ice"))
        }
    }
}

application {
    // Specify the main entry point for the application.
    mainClass.set("com.zeroc.demos.greeter.Server")
}
