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
        // This demo uses the nightly build of the Slice Tools plugin, published to the ZeroC Maven Nightly repository.
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-nightly/")
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
        // This demo uses the nightly build of Ice, published to the ZeroC Maven Nightly repository.
        maven {
            url = uri("https://download.zeroc.com/nexus/repository/maven-nightly/")
            content {
                includeGroupByRegex("com\\.zeroc.*")
            }
        }
    }
}

rootProject.name = "Ice Android Greeter"
include(":app")

includeBuild("../../build-logic")
