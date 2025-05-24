plugins {
    `kotlin-dsl`
    id("org.jetbrains.kotlin.jvm")
    //id("checkstyle")
    //id("org.openrewrite.rewrite") version "8.52.1"
}

group = "com.zeroc.convention"

java {
    sourceCompatibility = JavaVersion.VERSION_21
    targetCompatibility = JavaVersion.VERSION_21
}

dependencies {
    compileOnly(libraries.kotlin.gradlePlugin)
}

gradlePlugin { 
    plugins {
        register("formatAndLint") {
            id = "zeroc.convention"
            implementationClass = "FormatAndLintPlugin"
        }
    }
}

/*sourceSets.main.resources {
    srcDirs = ["src/zeroc/convention/properties"]
}*/
