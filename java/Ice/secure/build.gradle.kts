// Copyright (c) ZeroC, Inc.

subprojects {
    tasks.withType<JavaCompile>().configureEach {
        options.compilerArgs.add("-Xlint:all")
    }
}
