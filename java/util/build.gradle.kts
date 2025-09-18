// Copyright (c) ZeroC, Inc.

plugins {
    // Apply the java-library plugin to tell gradle this is a Java library.
    id("java-library")

    // Apply the Slice-tools plugin to enable Slice compilation.
    id("com.zeroc.ice.slice-tools") version "3.8.+"

    // Pull in our local 'convention plugin' to enable linting.
    id("zeroc-linting")
}
