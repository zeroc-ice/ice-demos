// Copyright (c) ZeroC, Inc.

plugins {
    // Register the Ice Builder plugin without applying it here.
    // The plugin will be applied to all subprojects in the `subprojects` block below.
    id("com.zeroc.ice.slice-tools") version "3.8.0-+" apply false
    id("checkstyle")
    id("org.openrewrite.rewrite") version "7.3.0"
}

allprojects {
    repositories {
        mavenCentral()
    }
}

subprojects {
    // Apply the application plugin to enable Java application support in subprojects.
    apply(plugin = "application")

    // Apply the Slice Tools for Ice and Java plugin to enable Slice compilation in all subprojects.
    apply(plugin = "com.zeroc.ice.slice-tools")

    apply(plugin = "checkstyle")

    repositories {
        // Add the ZeroC Nightly repository for downloading Ice runtime and development artifacts.
        maven("https://download.zeroc.com/nexus/repository/maven-nightly/")
        mavenCentral()
    } 

    checkstyle {
        setConfig(project.resources.text.fromFile(rootProject.file("${rootDir}/checkstyle/checkstyle.xml")))
        setToolVersion("10.21.4")
        setIgnoreFailures(false)
        setShowViolations(true)
    }

    // We want Checkstyle to generally run before Rewrite and to exclude files generated from Slice.
    project.getTasks().withType<Checkstyle>().let {
        it.forEach() { 
            var checkstyleTask = it
            checkstyleTask.configFile = rootProject.file("checkstyle/checkstyle.xml")
            rootProject.tasks.named("rewriteDryRun") { dependsOn(checkstyleTask) }
            rootProject.tasks.named("rewriteRun") { dependsOn(checkstyleTask) }
        }
    }
}

rewrite {
    activeRecipe(
        "org.openrewrite.staticanalysis.RemoveCallsToObjectFinalize",
        "org.openrewrite.staticanalysis.DefaultComesLast",
        "org.openrewrite.java.RemoveUnusedImports",
        "org.openrewrite.java.format.NormalizeFormat",
        "org.openrewrite.java.format.BlankLines",
        "org.openrewrite.java.format.Spaces",
        "org.openrewrite.java.format.NormalizeTabsOrSpaces",
        "org.openrewrite.java.format.TabsAndIndents",
        "org.openrewrite.java.format.NormalizeLineBreaks",
        "org.openrewrite.java.format.RemoveTrailingWhitespace",
        "org.openrewrite.staticanalysis.CodeCleanup"
    )
    checkstyleConfigFile = file("checkstyle/checkstyle.xml")
    activeStyle("com.zeroc.IceRewriteRules")
    setExportDatatables(true)
    exclusion(
        // OpenRewrite is technically capable of modifying Groovy and Kotlin files, but we are avoiding this for now.
        "**/*.gradle",
        "**/*.kts",
        // These are all generated files, and thus should not be subject to modification from OpenRewrite.
        "**/generated/**/*",
    )
}

dependencies {
    checkstyle("com.puppycrawl.tools:checkstyle:10.21.4")
    rewrite("org.openrewrite.recipe:rewrite-static-analysis:2.5.1")
    rewrite("org.openrewrite.recipe:rewrite-java-dependencies:1.31.0")
}

// Causes build failure when rewriteDryRun would make changes.
tasks.named("rewriteDryRun") {
    rewrite.failOnDryRunResults = true
}