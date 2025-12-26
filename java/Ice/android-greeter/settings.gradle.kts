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
        // Use the nightly build of the Slice Tools plugin from the ZeroC maven-3.9-nightly repository.
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-3.9-nightly/")
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
        // Use the nightly build of Ice from the ZeroC maven-3.9-nightly repository.
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-3.9-nightly/")
            content {
                includeGroupByRegex("com\\.zeroc.*")
            }
        }
    }
}

rootProject.name = "Ice Android Greeter"
include(":app")

includeBuild("../../build-logic")
