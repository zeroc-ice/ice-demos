// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the java-library plugin to tell gradle this is a Java library.
    id("java-library")

    // Pull in our local 'convention plugin' to enable linting.
    id("zeroc-linting")
}
