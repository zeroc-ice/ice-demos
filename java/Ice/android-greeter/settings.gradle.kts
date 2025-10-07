pluginManagement {
    repositories {
        google {
            content {
                includeGroupByRegex("com\\.android.*")
                includeGroupByRegex("com\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        mavenCentral()
        gradlePluginPortal()
        // Use the nightly build of the Slice Tools plugin from the Maven Central snapshots repository.
        maven {
            url = uri("https://central.sonatype.com/repository/maven-snapshots")
            content {
                includeGroupByRegex("com\\.zeroc.*")
            }
        }
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        // Use the nightly build of Ice from the Maven Central snapshots repository.
        maven {
            url = uri("https://central.sonatype.com/repository/maven-snapshots")
            content {
                includeGroupByRegex("com\\.zeroc.*")
            }
        }
    }
}

rootProject.name = "Ice Android Greeter"
include(":app")

includeBuild("../../build-logic")
