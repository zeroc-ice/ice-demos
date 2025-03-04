# Copyright (c) ZeroC, Inc.

if(NOT DEFINED Ice_WIN32_PLATFORM)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(Ice_WIN32_PLATFORM "x64" CACHE STRING "Ice library platform (x64 or Win32)")
  else()
    set(Ice_WIN32_PLATFORM "Win32" CACHE STRING "Ice library platform (x64 or Win32)")
  endif()
endif()

# Ice include directory
set(Ice_INCLUDE_DIR "${PACKAGE_PREFIX_DIR}/include" CACHE PATH "Path to the Ice include directory")

if(NOT EXISTS "${Ice_INCLUDE_DIR}")
  message(FATAL_ERROR "Ice include directory not found: ${Ice_INCLUDE_DIR}")
endif()

# Read Ice version variables from Ice/Config.h
if(NOT DEFINED Ice_VERSION)
  file(STRINGS "${Ice_INCLUDE_DIR}/Ice/Config.h" _ice_config_h_content REGEX "#define ICE_([A-Z]+)_VERSION ")

  if("${_ice_config_h_content}" MATCHES "#define ICE_STRING_VERSION \"([^\"]+)\"")
    set(Ice_VERSION "${CMAKE_MATCH_1}" CACHE STRING "Ice version")
  endif()

  if("${_ice_config_h_content}" MATCHES "#define ICE_SO_VERSION \"([^\"]+)\"")
    set(Ice_SO_VERSION "${CMAKE_MATCH_1}" CACHE STRING "Ice so version")
  endif()

  if("${_ice_config_h_content}" MATCHES "#define ICE_INT_VERSION ([0-9]+)")
    set(Ice_INT_VERSION "${CMAKE_MATCH_1}" CACHE STRING "Ice int version")
  endif()

  unset(_ice_config_h_content)
endif()

find_program(Ice_SLICE2CPP_EXECUTABLE slice2cpp
  HINTS ${PACKAGE_PREFIX_DIR}
  PATH_SUFFIXES bin ../../tools
  CACHE PATH "Path to the slice2cpp executable"
  NO_DEFAULT_PATH
  REQUIRED
)

# Add an imported target for slice2cpp
add_executable(Ice::slice2cpp IMPORTED)
set_target_properties(Ice::slice2cpp PROPERTIES
  IMPORTED_LOCATION "${Ice_SLICE2CPP_EXECUTABLE}"
)

find_path(Ice_SLICE_DIR
  NAMES Ice/Identity.ice
  HINTS ${PACKAGE_PREFIX_DIR}
  PATH_SUFFIXES slice share/ice/slice ../../slice
  CACHE PATH "Path to the Ice Slice files directory"
  NO_DEFAULT_PATH
  REQUIRED)

# Adds an Ice:<component> target with the specified link libraries
function(add_ice_target component link_libraries)
  add_library(Ice::${component} SHARED IMPORTED)
  set_target_properties(Ice::${component} PROPERTIES
    INTERFACE_COMPILE_FEATURES "cxx_std_17"
    INTERFACE_INCLUDE_DIRECTORIES "${Ice_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "${link_libraries}"
  )

  if(WIN32)
    if(Ice_${component}_LIBRARY_RELEASE)
      set_target_properties(Ice::${component} PROPERTIES
        IMPORTED_CONFIGURATIONS RELEASE
        IMPORTED_IMPLIB_RELEASE "${Ice_${component}_IMPLIB_RELEASE}"
        IMPORTED_LOCATION_RELEASE "${Ice_${component}_LIBRARY_RELEASE}"
      )
    endif()

    if(Ice_${component}_LIBRARY_DEBUG)
      set_target_properties(Ice::${component} PROPERTIES
        IMPORTED_CONFIGURATIONS DEBUG
        IMPORTED_IMPLIB_DEBUG "${Ice_${component}_IMPLIB_DEBUG}"
        IMPORTED_LOCATION_DEBUG "${Ice_${component}_LIBRARY_DEBUG}"
      )
    endif()

  else()
    if(Ice_${component}_LIBRARY)
      set_target_properties(Ice::${component} PROPERTIES
        IMPORTED_LOCATION "${Ice_${component}_LIBRARY}"
      )
    endif()
  endif()

endfunction()

function(add_ice_library component link_libraries)

  if(WIN32)
    # Find Release and Debug libraries on Windows
    find_library(Ice_${component}_IMPLIB_RELEASE
      NAMES ${component}${Ice_SO_VERSION}
      PATHS "${PACKAGE_PREFIX_DIR}/lib/${Ice_WIN32_PLATFORM}/Release"
      NO_DEFAULT_PATH
    )

    find_library(Ice_${component}_IMPLIB_DEBUG
      NAMES ${component}d ${component}${Ice_SO_VERSION}d
      PATHS "${PACKAGE_PREFIX_DIR}/lib/${Ice_WIN32_PLATFORM}/Debug"
      NO_DEFAULT_PATH
    )

    find_file(Ice_${component}_LIBRARY_RELEASE
      NAMES ${component}${Ice_SO_VERSION}.dll
      PATHS "${PACKAGE_PREFIX_DIR}/bin/${Ice_WIN32_PLATFORM}/Release"
      NO_DEFAULT_PATH
    )

    find_file(Ice_${component}_LIBRARY_DEBUG
      NAMES ${component}${Ice_SO_VERSION}d.dll
      PATHS "${PACKAGE_PREFIX_DIR}/bin/${Ice_WIN32_PLATFORM}/Debug"
      NO_DEFAULT_PATH
    )
  else()
    # Linux and macOS only have one library configuration
    find_library(
      Ice_${component}_LIBRARY_RELEASE
      NAMES ${component}
      PATHS "${PACKAGE_PREFIX_DIR}/lib" "${PACKAGE_PREFIX_DIR}/lib/${CMAKE_LIBRARY_ARCHITECTURE}"
      NO_DEFAULT_PATH
    )
  endif()

  # Select the appropriate library configuration based on platform and build type
  include(SelectLibraryConfigurations)
  select_library_configurations(Ice_${component})

  if(Ice_${component}_LIBRARY)
    # Set the Ice_<component>_FOUND variable to TRUE so that find_package_handle_standard_args
    # will consider the component found
    set(Ice_${component}_FOUND TRUE PARENT_SCOPE)
    add_ice_target(${component} "${link_libraries}")
  endif()

endfunction()

include(CMakeFindDependencyMacro)
find_package(Threads REQUIRED QUIET)

add_ice_library(Ice Threads::Threads)
add_ice_library(DataStorm Ice::Ice)
add_ice_library(Glacier2 Ice::Ice)
add_ice_library(IceBox Ice::Ice)
add_ice_library(IceDiscovery Ice::Ice)
add_ice_library(IceGrid Ice::Ice Ice::Glacier2)
add_ice_library(IceLocatorDiscovery Ice::Ice)
add_ice_library(IceStorm Ice::Ice)
add_ice_library(IceBT Ice::Ice)

# Use this CMake helper method to handle REQUIRED, QUIET and version related arguments to find_package
find_package_handle_standard_args(Ice
  VERSION_VAR Ice_VERSION
  HANDLE_COMPONENTS)
