import org.gradle.api.Project
import org.gradle.api.Plugin


class FormatAndLintPlugin : Plugin<Project> {
    override fun apply(target: Project) {
        with(target) {
            apply("com.puppycrawl.tools:checkstyle:10.21.4") //version "10.21.4"
            apply("org.openrewrite:rewrite-java:8.52.1") //version "8.52.1"
        }

        extensions.configure<LibraryExtension> {
            
        }
    }
}

internal fun Project.configureCheckstyle(commonExtension: CommonExtension<*, *, *, *>) {
    commonExtension.apply {
        setConfig(project.resources.text.fromFile(rootProject.file("${rootDir}/checkstyle/checkstyle.xml")))
        setToolVersion("10.21.4")
        setIgnoreFailures(false)
        setShowViolations(true)
    }
     
    val libraries = extensions.getByType<VersionCatalogsExtension>().named("libraries")

    dependencies {
        add("checkstyleCore", libraries.findLibrary("fixerLibraries.checkstyleLibrary"))
        //checkstyle("com.puppycrawl.tools:checkstyle:10.21.4")
    }
}

internal fun Project.configureOpenRewrite(commonExtension: CommonExtension<*, *, *, *>) {
    commonExtension.apply {
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

    val libraries = extensions.getByType<VersionCatalogsExtension>().named("libraries")

    dependencies {
        add("staticAnalysis", libraries.findLibrary("fixerLibraries.rewriteAnalysisLibrary"))
        add("javaDependencies", libraries.findLibrary("fixerLibraries.rewriteJavaDependencies"))
    }
    //rewrite("org.openrewrite.recipe:rewrite-static-analysis:2.5.1")
    //rewrite("org.openrewrite.recipe:rewrite-java-dependencies:1.31.0")
}