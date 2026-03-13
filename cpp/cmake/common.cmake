# Copyright (c) ZeroC, Inc.

cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ice for C++ on Windows is shipped as a NuGet package.
if(WIN32)
  include("${CMAKE_CURRENT_LIST_DIR}/ice-nuget.cmake")

  # Ice PDB files (which are not included in the NuGet package) are required for getting stack traces at runtime.
  # If ICE_COPY_PDB is enabled, we download the PDB files at configure time and copy them to the target's output
  # directory at build time.
  include("${CMAKE_CURRENT_LIST_DIR}/ice-pdbs.cmake")
endif()

# We use these flags over presets to avoid having to create a CMakePreset.json file in every project.
# If you want to disable, set this option to OFF (`-DICE_DEMO_WARNINGS=OFF`) when configuring CMake.
option(ICE_DEMO_WARNINGS "Ice Demo C++ warning flags" ON)
if(ICE_DEMO_WARNINGS)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wredundant-decls -Wdeprecated)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
      -Wall
      -Wextra
      -Wredundant-decls
      -Wdeprecated
      -Wstrict-prototypes
      -Wconversion
      -Wdocumentation
      -Wreorder-init-list
    )
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4)
  endif()
endif()


find_package(Ice REQUIRED CONFIG)
