# Copyright (c) ZeroC, Inc.

# get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
# TEMPORARY until CMake config is shipped in packaging
if(NOT DEFINED PACKAGE_PREFIX_DIR)
  if(APPLE)
    set(PACKAGE_PREFIX_DIR "/opt/homebrew" CACHE PATH "Path to Brew prefix directory")
  elseif(UNIX)
    set(PACKAGE_PREFIX_DIR "/usr" CACHE PATH "Path to Ice installation directory")
  elseif(WIN32)
    message(FATAL_ERROR "Please set the PACKAGE_PREFIX_DIR variable to the NuGet package directory")
  endif()
endif()

include("${CMAKE_CURRENT_LIST_DIR}/IceTargets.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/slice2cpp.cmake")
