// Copyright (c) ZeroC, Inc.

dependencies {
    // Add the Ice library as an implementation dependency.
    implementation("com.zeroc:ice:3.8.0-+")
}

application {
    // Specify the main entry point for the application.
    mainClass.set("com.example.ice.forwarder.forwardingserver.Server")
    applicationDefaultJvmArgs = listOf("-ea") // Enables assertions
}
