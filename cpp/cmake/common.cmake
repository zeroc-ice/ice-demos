# Copyright (c) ZeroC, Inc.

cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ice for C++ on Windows is shipped as a NuGet package.
if(WIN32)
  set(Ice_NUGET_NAME "ZeroC.Ice.Cpp")
  file(GLOB _ice_nuget_dirs "${CMAKE_CURRENT_LIST_DIR}/packages/${Ice_NUGET_NAME}.*")

  if(NOT _ice_nuget_dirs)

    # Check if the nuget command line tool is available (either in the PATH or previously downloaded).
    # If not download it.
    find_program(NUGET_EXE nuget HINTS "${CMAKE_CURRENT_LIST_DIR}/packages" DOC "Path to nuget.exe")
    if(NOT NUGET_EXE)
      set(NUGET_URL "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe" CACHE STRING "NuGet command line tool URL")
      message(STATUS "Downloading NuGet command line tool: ${NUGET_URL}")
      file(DOWNLOAD ${NUGET_URL} ${CMAKE_CURRENT_LIST_DIR}/packages/nuget.exe)
      # Clear the failed find_program result and set the downloaded path
      unset(NUGET_EXE CACHE)
      set(NUGET_EXE "${CMAKE_CURRENT_LIST_DIR}/packages/nuget.exe" CACHE FILEPATH "Path to downloaded nuget.exe")
    endif()

    # Download the Ice NuGet package using the nuget command line tool.
    message(STATUS "Downloading Ice NuGet package: ${Ice_NUGET_NAME}")
    set(ICE_NUGET_SOURCE "https://download.zeroc.com/nexus/repository/nuget-3.9-nightly/" CACHE STRING "Ice NuGet package source")
    execute_process(
      COMMAND ${NUGET_EXE} install -Source ${ICE_NUGET_SOURCE} -OutputDirectory ${CMAKE_CURRENT_LIST_DIR}/packages ${Ice_NUGET_NAME} -Prerelease
      RESULT_VARIABLE nuget_result
      OUTPUT_VARIABLE nuget_output
      ERROR_VARIABLE nuget_error)
    if(nuget_result)
      message(FATAL_ERROR "Failed to download Ice NuGet package: ${nuget_error}")
    endif()

    file(GLOB _ice_nuget_dirs "${CMAKE_CURRENT_LIST_DIR}/packages/${Ice_NUGET_NAME}.*")
    if(NOT _ice_nuget_dirs)
      message(FATAL_ERROR "NuGet install succeeded but no ${Ice_NUGET_NAME}.<version> directory was found")
    endif()
  endif()

  list(SORT _ice_nuget_dirs COMPARE NATURAL ORDER DESCENDING)
  list(GET _ice_nuget_dirs 0 Ice_NUGET_DIR)

  get_filename_component(_ice_nuget_dir_name "${Ice_NUGET_DIR}" NAME)
  string(LENGTH "${Ice_NUGET_NAME}." _ice_nuget_prefix_len)
  string(SUBSTRING "${_ice_nuget_dir_name}" ${_ice_nuget_prefix_len} -1 Ice_NUGET_VERSION)
  # Not cached: always derived from the resolved directory so it can't go stale.
  set(Ice_NUGET_VERSION "${Ice_NUGET_VERSION}")

  # Set Ice_Root to the Ice NuGet package path. This is a special variable
  # that is used by CMake to find the Ice CMake config.
  set(Ice_ROOT "${Ice_NUGET_DIR}" CACHE PATH "Path to Ice installation directory")

  option(ICE_USE_PDB "Download and copy Ice PDB files alongside DLLs" OFF)
  if(ICE_USE_PDB)
    set(ICE_PDB_DIR "${CMAKE_CURRENT_LIST_DIR}/pdb" CACHE PATH "Directory where Ice PDB files are extracted")
    set(ICE_PDB_URL "https://download.zeroc.com/ice/nightly/3.9/windows-symbols-sources-${Ice_NUGET_VERSION}.zip" CACHE STRING "URL to download Ice PDB ZIP archive")
    if(NOT EXISTS "${ICE_PDB_DIR}")
      include("${CMAKE_CURRENT_LIST_DIR}/download-ice-pdbs.cmake")
    endif()
  endif()
endif()

# Copy Ice PDB files that match the DLLs in the target's output directory.
# PDBs are selected based on architecture and Debug/Release configuration.
# This is a no-op when ICE_USE_PDB is OFF or on non-Windows platforms.
function(ice_copy_pdbs target)
  if(WIN32 AND ICE_USE_PDB AND ICE_PDB_DIR)
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND "${CMAKE_COMMAND}"
        -DICE_PDB_SOURCE_DIR="${ICE_PDB_DIR}/${Ice_WIN32_PLATFORM}/$<IF:$<CONFIG:Debug>,Debug,Release>"
        -DTARGET_DIR="$<TARGET_FILE_DIR:${target}>"
        "-DRUNTIME_DLLS=$<TARGET_RUNTIME_DLLS:${target}>;$<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>"
        -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/copy-ice-pdbs.cmake"
    )
  endif()
endfunction()

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
