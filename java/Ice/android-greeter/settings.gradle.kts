pluginManagement {
    repositories {
        google {
            content {
                includeGroupByRegex("com\\.android.*")
                includeGroupByRegex("com\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        maven("https://download.zeroc.com/nexus/repository/maven-nightly/")
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        // This demo uses the latest Ice nightly build published to the maven central snapshots repository.
        maven("https://download.zeroc.com/nexus/repository/maven-nightly/")
        google()
        mavenCentral()
    }
}

rootProject.name = "Ice Android Greeter"
include(":app")

includeBuild("../../build-logic")
