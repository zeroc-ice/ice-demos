# Copyright (c) ZeroC, Inc.

cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
  set(Ice_NUGET_NAME "zeroc.ice.v143")
  set(Ice_NUGET_DIR "${CMAKE_CURRENT_LIST_DIR}/packages/${Ice_NUGET_NAME}")

  if(NOT EXISTS ${Ice_NUGET_DIR})
    message(STATUS "Downloading Ice NuGet package")

    set(ICE_NUGET_SOURCE "https://download.zeroc.com/nexus/repository/nuget-nightly/" CACHE STRING "Ice NuGet package source")

    execute_process(
      COMMAND nuget install -Source ${ICE_NUGET_SOURCE} -OutputDirectory ${CMAKE_CURRENT_LIST_DIR}/packages ${Ice_NUGET_NAME} -Prerelease -ExcludeVersion
      RESULT_VARIABLE nuget_result
      OUTPUT_VARIABLE nuget_output
      ERROR_VARIABLE nuget_error)
    if(nuget_result)
      message(FATAL_ERROR "Failed to download Ice NuGet package: ${nuget_error}")
    endif()

  endif()

  # Set Ice_HOME to the path where the Ice NuGet package was downloaded
  set(PACKAGE_PREFIX_DIR "${Ice_NUGET_DIR}" CACHE PATH "Path to Ice installation directory")
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

set(Ice_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path to Ice CMake configuration file")

find_package(Ice REQUIRED CONFIG)
