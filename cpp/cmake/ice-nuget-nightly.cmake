# Copyright (c) ZeroC, Inc.

# Nightly version of ice-nuget.cmake.
# Downloads the latest prerelease Ice NuGet package from the 3.8 nightly feed.
# Sets Ice_ROOT and Ice_NUGET_VERSION for use by other scripts.

set(Ice_NUGET_NAME "ZeroC.Ice.Cpp")
file(GLOB _ice_nuget_dirs "${CMAKE_CURRENT_LIST_DIR}/packages/${Ice_NUGET_NAME}.*")

if(NOT _ice_nuget_dirs)
  # Check if the nuget command line tool is available (either in the PATH or previously downloaded).
  # If not download it.
  find_program(NUGET_EXE nuget HINTS "${CMAKE_CURRENT_LIST_DIR}/packages" DOC "Path to nuget.exe")
  if(NOT NUGET_EXE)
    set(NUGET_URL "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe" CACHE STRING "NuGet command line tool URL")
    message(STATUS "Downloading NuGet command line tool: ${NUGET_URL}")
    file(DOWNLOAD "${NUGET_URL}" "${CMAKE_CURRENT_LIST_DIR}/packages/nuget.exe")
    # Clear the failed find_program result and set the downloaded path
    unset(NUGET_EXE CACHE)
    set(NUGET_EXE "${CMAKE_CURRENT_LIST_DIR}/packages/nuget.exe" CACHE FILEPATH "Path to downloaded nuget.exe")
  endif()

  # Download the Ice NuGet package using the nuget command line tool.
  message(STATUS "Downloading Ice NuGet package: ${Ice_NUGET_NAME} (nightly)")
  set(ICE_NUGET_SOURCE "https://download.zeroc.com/nexus/repository/nuget-3.8-nightly/" CACHE STRING "Ice NuGet package source")
  execute_process(
    COMMAND "${NUGET_EXE}" install -Source "${ICE_NUGET_SOURCE}" -OutputDirectory "${CMAKE_CURRENT_LIST_DIR}/packages" "${Ice_NUGET_NAME}" -Prerelease
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

# Extract version from directory name.
get_filename_component(_ice_nuget_dir_name "${Ice_NUGET_DIR}" NAME)
string(LENGTH "${Ice_NUGET_NAME}." _ice_nuget_prefix_len)
string(SUBSTRING "${_ice_nuget_dir_name}" ${_ice_nuget_prefix_len} -1 Ice_NUGET_VERSION)

# Set URL for the PDB files that matches the nightly NuGet package.
set(ICE_PDB_URL "https://download.zeroc.com/ice/nightly/3.8/windows-indexed-pdbs-${Ice_NUGET_VERSION}.zip" CACHE STRING "URL to download Ice PDB ZIP archive")

# Set Ice_Root to the Ice NuGet package path. This is a special variable
# that is used by CMake to find the Ice CMake config.
set(Ice_ROOT "${Ice_NUGET_DIR}" CACHE PATH "Path to Ice installation directory")
