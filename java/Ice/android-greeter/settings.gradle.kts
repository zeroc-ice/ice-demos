pluginManagement {
    repositories {
        google {
            content {
                includeGroupByRegex("com\\.android.*")
                includeGroupByRegex("com\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        maven("https://central.sonatype.com/repository/maven-snapshots/")
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        // This demo uses the latest Ice nightly build published in ZeroC's maven-nightly repository.
        maven("https://central.sonatype.com/repository/maven-snapshots/")
        google()
        mavenCentral()
    }
}

rootProject.name = "Ice Android Greeter"
include(":app")

includeBuild("../../build-logic")
