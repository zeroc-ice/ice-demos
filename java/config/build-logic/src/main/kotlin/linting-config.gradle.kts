// Copyright (c) ZeroC, Inc.

val runningInCi = providers.environmentVariable("CI").isPresent

plugins {
    // Apply the checkstyle plugin to find formatting violations.
    id("checkstyle")

    // Apply the rewrite plugin to find (and sometimes automatically fix) lint violations.
    id("org.openrewrite.rewrite")
}

dependencies {
    // The 'rewrite' uses a modular plug-in system for linting. We pull in the recipes we want to use.
    rewrite(
        "org.openrewrite.recipe:rewrite-static-analysis:2.5.1"
    )
}

// Configure the checkstyle plugin.
checkstyle {
    toolVersion = "10.21.4"

    configDirectory.set(file("$rootDir/../../config/checkstyle/"))

    // If we're running in CI, we want the build to fail if any warnings are emitted.
    // This doesn't affect what checkstyle emits, just whether it 'fails' or 'succeeds' from Gradle's perspective.
    if (runningInCi) {
        maxWarnings = 0
    }
}

rewrite {
    activeRecipe("com.zeroc.IceRewriteRecipes")
    activeStyle("com.zeroc.IceRewriteStyle")
    configFile = file("$rootDir/../../config/rewrite.yml")

    exclusion(
        // Don't check the generated code.
        "**/build/generated/**/*",
        // Prevent 'rewrite' from trying (and failing) to format our Kotlin code with Java rules.
        "*.kts",
    )
}

// Set whether or not 'rewriteDryRun' should be considered 'failed' when it would make changes.
tasks.named("rewriteDryRun").configure {
    rewrite.failOnDryRunResults = runningInCi
}

// We always want to pass "-Xlint:all" when running 'javac'. This enables all its built-in lint checking.
tasks.withType<JavaCompile>().configureEach {
    options.compilerArgs.add("-Xlint:all")
}

// Ensure assertions are always enabled when executing java code.
tasks.withType<JavaExec>().configureEach {
    enableAssertions = true
}
