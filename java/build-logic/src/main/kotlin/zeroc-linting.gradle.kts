// Copyright (c) ZeroC, Inc.

val runningInCi = providers.environmentVariable("CI").isPresent

plugins {
    // Apply the checkstyle plugin to find formatting violations.
    id("checkstyle")

    // Apply the rewrite plugin to find (and sometimes automatically fix) lint violations.
    id("org.openrewrite.rewrite")
}

dependencies {
    // 'rewrite' uses a modular plug-in system for linting. We pull in the recipes we want to use.
    rewrite(
        "org.openrewrite.recipe:rewrite-static-analysis:2.5.1"
    )
}

checkstyle {
    toolVersion = "10.21.4"

    configDirectory.set(file("$rootDir/../../config/checkstyle/"))

    // If we're running in CI, we want the build to fail if any warnings are emitted.
    // This doesn't affect what checkstyle emits, just whether it 'fails' or 'succeeds' from Gradle's perspective.
    if (runningInCi) {
        maxWarnings = 0
    }
}

tasks.withType<Checkstyle>().configureEach {
    // Ensure that running checkstyle multiple times always yields the same output. Otherwise, after an initial run,
    // the checkstyle task will be marked "UP TO DATE", and subsequent runs will skip over it.
    outputs.upToDateWhen { false }
    doNotTrackState("Always run Checkstyle")
}

rewrite {
    activeRecipe("com.zeroc.IceRewriteRecipes")
    activeStyle("com.zeroc.IceRewriteStyle")
    configFile = file("$rootDir/../../config/rewrite.yml")

    exclusion(
        // Don't check the generated code.
        "**/build/generated/**/*",
        // Prevent 'rewrite' from trying (and failing) to format our Kotlin code with Java rules.
        "**/*.kts",
        // Skip kotlin files, since we don't have any Kotlin rewrite recipes.
        "**/*.kt",
    )
}

// Fail the build when 'rewriteDryRun' wants to make changes to the code,
// and print those required changes in the build output for clarity.
tasks.withType<org.openrewrite.gradle.RewriteDryRunTask>().configureEach {
    doFirst {
        // Delete the old report file if present
        val reportFile = file(reportPath)
        if (reportFile.exists()) {
            reportFile.delete()
        }
    }

    doLast {
        if (file(reportPath).exists()) {
            print("\n\n" + java.nio.file.Files.readString(reportPath))

            if (runningInCi) {
                throw RuntimeException("Applying recipes would make changes. See logs for more details.")
            }
        }
    }
}

// We always want to pass "-Xlint:all" when running 'javac'. This enables all its built-in lint checking.
tasks.withType<JavaCompile>().configureEach {
    options.compilerArgs.add("-Xlint:all")
}

// Ensure assertions are always enabled when executing java code.
tasks.withType<JavaExec>().configureEach {
    enableAssertions = true
}
