
if (NOT Ice_HOME)
  if (DEFINED ENV{ICE_HOME})
    set(Ice_HOME $ENV{ICE_HOME} CACHE PATH "Path to the Ice installation directory")
  endif()
endif()

include(CMakeFindDependencyMacro)
find_package(Threads REQUIRED)

if(NOT DEFINED Ice_ARCHITECTURE)
  if (CMAKE_LIBRARY_ARCHITECTURE)
    set(Ice_ARCHITECTURE ${CMAKE_LIBRARY_ARCHITECTURE} CACHE STRING "Library architecture")
  elseif(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(Ice_ARCHITECTURE "x64" CACHE STRING "Windows x64")
    else()
      set(Ice_ARCHITECTURE "Win32" CACHE STRING "Windows x86")
    endif()
  endif()
endif()

list(APPEND ice_include_path_suffixes "include")
list(APPEND ice_bin_path_suffixes "bin")
list(APPEND ice_lib_path_suffixes "lib")

if(WIN32)
  # NuGet packages
  list(APPEND ice_include_path_suffixes "build/native/include")
  list(APPEND ice_bin_path_suffixes "tools")
  list(APPEND ice_bin_path_suffixes_release "build/native/bin/${Ice_ARCHITECTURE}/Release")
  list(APPEND ice_bin_path_suffixes_debug "build/native/bin/${Ice_ARCHITECTURE}/Debug")
  list(APPEND ice_lib_path_suffixes_release "build/native/lib/${Ice_ARCHITECTURE}/Release")
  list(APPEND ice_lib_path_suffixes_debug "build/native/lib/${Ice_ARCHITECTURE}/Debug")
endif()

# Ice include directory
if(NOT DEFINED Ice_INCLUDE_DIR)
  find_path(Ice_INCLUDE_DIR NAMES Ice/Ice.h HINTS ${Ice_HOME} PATH_SUFFIXES ${ice_include_path_suffixes} CACHE PATH "Path to the Ice include directory")
endif()

# Ice include directories
if(NOT DEFINED Ice_INCLUDE_DIRS)
  set(Ice_INCLUDE_DIRS ${Ice_INCLUDE_DIR})
  set(Ice_INCLUDE_DIRS ${Ice_INCLUDE_DIRS} CACHE STRING "Ice include directories")
endif()

# Read Ice version variables from Ice/Config.h
if (NOT DEFINED Ice_VERSION)
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

if(NOT DEFINED Ice_SLICE2CPP_EXECUTABLE)
  find_program(Ice_SLICE2CPP_EXECUTABLE slice2cpp
    HINTS ${Ice_HOME}
    PATH_SUFFIXES ${ice_bin_path_suffixes}
    CACHE PATH "Path to the slice2cpp executable")
endif()

if(NOT DEFINED Ice_SLICE_DIR)
  find_path(Ice_SLICE_DIR NAMES Ice/Identity.ice HINTS ${Ice_HOME} PATH_SUFFIXES slice share/ice/slice CACHE PATH "Path to the Ice Slice files directory")
endif()

# Create a target for each requested component
foreach(component ${Ice_FIND_COMPONENTS})
  if(NOT TARGET Ice::${component})

    # Find the dll and import library for the component for both Debug and Release configurations.
    if (WIN32)
      # Set the import libraries to the library found above
      find_library(Ice_${component}_IMPLIB_RELEASE
        NAMES ${component} ${component}${Ice_SO_VERSION}
        HINTS ${Ice_HOME}
        PATH_SUFFIXES ${ice_lib_path_suffixes_release}
      )

      find_library(Ice_${component}_IMPLIB_DEBUG
        NAMES ${component}d ${component}${Ice_SO_VERSION}d
        HINTS ${Ice_HOME}
        PATH_SUFFIXES lib ${ice_lib_path_suffixes_debug}
      )

      find_file(Ice_${component}_LIBRARY_RELEASE
        NAMES ${component}${Ice_SO_VERSION}.dll
        HINTS ${Ice_HOME}
        PATH_SUFFIXES ${ice_bin_path_suffixes_release}
      )

      find_file(Ice_${component}_LIBRARY_DEBUG
        NAMES ${component}${Ice_SO_VERSION}d.dll
        HINTS ${Ice_HOME}
        PATH_SUFFIXES ${ice_bin_path_suffixes_debug}
      )

    else()
      # On Linux/macOS, there is only one library for both Debug and Release configurations.
      find_library(Ice_${component}_LIBRARY_RELEASE
        NAMES ${component} ${component}${Ice_SO_VERSION}
        HINTS ${Ice_HOME}
        PATH_SUFFIXES ${ice_lib_path_suffixes}
      )
    endif()

    # Select the appropriate library configuration based on platform and build type
    include(SelectLibraryConfigurations)
    select_library_configurations(Ice_${component})

    if(Ice_${component}_LIBRARY)
      # Create an imported target for the component
      add_library(Ice::${component} SHARED IMPORTED)
      set_target_properties(Ice::${component} PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Ice_INCLUDE_DIRS}"
      )

      # Add the library to the target. On Windows we set both the IMPORTED_LOCATION and IMPORTED_IMPLIB properties.
      # This necessary to support CMake's TARGET_RUNTIME_DLLS generator expression.
      # On other platforms, we only set the IMPORTED_LOCATION property.
      if (WIN32)
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
        set_target_properties(Ice::${component} PROPERTIES
          IMPORTED_LOCATION "${Ice_${component}_LIBRARY}"
        )
      endif()
    endif()
  endif()
endforeach()

if(Ice_DEBUG)
  message(STATUS "Ice_VERSION: ${Ice_VERSION}")
  message(STATUS "Ice_SO_VERSION: ${Ice_SO_VERSION}")
  message(STATUS "Ice_INT_VERSION: ${Ice_INT_VERSION}")
  message(STATUS "Ice_HOME: ${Ice_HOME}")
  message(STATUS "Ice_SLICE2CPP_EXECUTABLE: ${Ice_SLICE2CPP_EXECUTABLE}")
  message(STATUS "Ice_SLICE_DIR directory: ${Ice_SLICE_DIR}")
  message(STATUS "Ice_INCLUDE_DIR: ${Ice_INCLUDE_DIR}")
  message(STATUS "Ice_INCLUDE_DIRS: ${Ice_INCLUDE_DIRS}")

  foreach(component ${Ice_FIND_COMPONENTS})
    message(STATUS "Ice_${component}_LIBRARY: ${Ice_${component}_LIBRARY}")
    if(Ice_${component}_LIBRARY_RELEASE)
      message(STATUS "Ice_${component}_LIBRARY_RELEASE: ${Ice_${component}_LIBRARY_RELEASE}")
    endif()
    if(Ice_${component}_LIBRARY_DEBUG)
      message(STATUS "Ice_${component}_LIBRARY_DEBUG: ${Ice_${component}_LIBRARY_DEBUG}")
    endif()

    if(Ice_${component}_IMPLIB_RELEASE)
      message(STATUS "Ice_${component}_IMPLIB_RELEASE: ${Ice_${component}_IMPLIB_RELEASE}")
    endif()
    if(Ice_${component}_IMPLIB_DEBUG)
      message(STATUS "Ice_${component}_IMPLIB_DEBUG: ${Ice_${component}_IMPLIB_DEBUG}")
    endif()
  endforeach()
endif()

unset(ice_include_path_suffixes)
unset(ice_bin_path_suffixes)
unset(ice_lib_path_suffixes)
unset(ice_lib_path_suffixes_release)
unset(ice_lib_path_suffixes_debug)

# Use `find_package_handle_standard_args` for better error handling
find_package_handle_standard_args(Ice
  REQUIRED_VARS Ice_SLICE_DIR Ice_INCLUDE_DIRS Ice_SLICE2CPP_EXECUTABLE
  HANDLE_COMPONENTS
)

# Function to generate C++ source files from Slice (.ice) files for a target using slice2cpp
# The target must have the Slice files in its sources
# The generated files are added to the target sources
# Usage:
# add_executable(a_target source1.cpp source2.ice source3.ice)
# slice2cpp_generate(a_target)
function(slice2cpp_generate target)

  # Get the list of source files for the target
  get_target_property(sources ${target} SOURCES)

  # Get the output directory for the generated files
  set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/generated/${target})
  file(RELATIVE_PATH output_dir_relative ${CMAKE_CURRENT_LIST_DIR} ${output_dir})

  # Create a directory to store the generated files
  make_directory(${output_dir})

  # Add the generated headers files to the target include directories
  target_include_directories(${target} PRIVATE ${output_dir})

  # Process each Slice (.ice) file in the source list
  # 1. Run the slice2cpp command to generate the header and source files
  # 2. Add the generated files to the target sources
  foreach(file IN LISTS sources)
    if(file MATCHES "\\.ice$")

      get_filename_component(slice_file_name ${file} NAME_WE)
      get_filename_component(slice_file_path ${file} ABSOLUTE)

      set(output_files ${output_dir}/${slice_file_name}.h ${output_dir}/${slice_file_name}.cpp)

      add_custom_command(
        OUTPUT ${output_files}
        COMMAND ${Ice_SLICE2CPP_EXECUTABLE} -I${Ice_SLICE_DIR} ${slice_file_path} --output-dir ${output_dir}
        DEPENDS ${slice_file_path}
        COMMENT "Compiling Slice ${file} -> ${output_dir_relative}/${slice_file_name}.cpp ${output_dir_relative}/${slice_file_name}.h"
      )

      target_sources(${target} PRIVATE ${output_files})

    endif()
  endforeach()
endfunction()
