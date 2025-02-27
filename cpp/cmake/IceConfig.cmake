
if (NOT Ice_HOME)
  if (DEFINED ENV{ICE_HOME})
    set(Ice_HOME $ENV{ICE_HOME} CACHE PATH "Path to the Ice installation directory")
  endif()
endif()

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

if(EXISTS ${Ice_HOME}/cpp)
  set(Ice_SOURCE_BUILD ON)
endif()

if(Ice_SOURCE_BUILD)
  list(APPEND ice_include_path_suffixes "cpp/include")
  list(APPEND ice_bin_path_suffixes "cpp/bin")
  list(APPEND ice_lib_path_suffixes_release "cpp/lib")

  if (Ice_ARCHITECTURE)
    list(APPEND ice_lib_path_suffixes_release "cpp/lib/${Ice_ARCHITECTURE}")
  endif()

  if(WIN32)
    list(APPEND ice_bin_path_suffixes "cpp/bin/${Ice_ARCHITECTURE}/Release" "cpp/bin/${Ice_ARCHITECTURE}/Debug")
    list(APPEND ice_lib_path_suffixes_release "cpp/lib/${Ice_ARCHITECTURE}/Release")
    list(APPEND ice_lib_path_suffixes_debug "cpp/lib/${Ice_ARCHITECTURE}/Debug")
  endif()

else()

  list(APPEND ice_include_path_suffixes "include")
  list(APPEND ice_bin_path_suffixes "bin")
  list(APPEND ice_lib_path_suffixes_release "lib")

  if(WIN32)
    # NuGet packages
    list(APPEND ice_include_path_suffixes "build/native/include")
    list(APPEND ice_bin_path_suffixes "tools")
    list(APPEND ice_lib_path_suffixes_release "build/native/lib/${Ice_ARCHITECTURE}/Release")
    list(APPEND ice_lib_path_suffixes_debug "build/native/lib/${Ice_ARCHITECTURE}/Debug")
  endif()

endif()

# Ice include directory
if(NOT DEFINED Ice_INCLUDE_DIR)
  find_path(Ice_INCLUDE_DIR NAMES Ice/Ice.h HINTS ${Ice_HOME} PATH_SUFFIXES ${ice_include_path_suffixes} CACHE PATH "Path to the Ice include directory")
endif()

# Ice include directories include the generated directory(s)
if(NOT DEFINED Ice_INCLUDE_DIRS)
  set(Ice_INCLUDE_DIRS ${Ice_INCLUDE_DIR})

  if(Ice_SOURCE_BUILD)
    #  Only the Windows build has separate Debug and Release directories for the generated files
    if(WIN32)
      set(Ice_GENERATED_INCLUDE_DIR_RELEASE ${Ice_INCLUDE_DIR}/generated/${Ice_ARCHITECTURE}/Release)
      if (EXISTS ${Ice_GENERATED_INCLUDE_DIR_RELEASE})
        list(APPEND Ice_INCLUDE_DIRS $<$<CONFIG:Release>:${Ice_GENERATED_INCLUDE_DIR_RELEASE}>)
      endif()

      set(Ice_GENERATED_INCLUDE_DIR_DEBUG ${Ice_INCLUDE_DIR}/generated/${Ice_ARCHITECTURE}/Debug)
      if (EXISTS ${Ice_GENERATED_INCLUDE_DIR_DEBUG})
        list(APPEND Ice_INCLUDE_DIRS $<$<CONFIG:Debug>:${Ice_GENERATED_INCLUDE_DIR_DEBUG}>)
      endif()
    else()
        list(APPEND Ice_INCLUDE_DIRS ${Ice_INCLUDE_DIR}/generated)
    endif()
  endif()

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

foreach(component ${Ice_FIND_COMPONENTS})
  if(NOT TARGET Ice::${component})

    find_library(Ice_${component}_LIBRARY_RELEASE
      NAMES ${component} ${component}${Ice_SO_VERSION}
      HINTS ${Ice_HOME}
      PATH_SUFFIXES ${ice_lib_path_suffixes_release}
    )

    find_library(Ice_${component}_LIBRARY_DEBUG
      NAMES ${component}d ${component}${Ice_SO_VERSION}d
      HINTS ${Ice_HOME}
      PATH_SUFFIXES lib ${ice_lib_path_suffixes_debug}
    )

    # Select the appropriate library configuration based on platform and build type
    include(SelectLibraryConfigurations)
    select_library_configurations(Ice_${component})

    if(Ice_${component}_LIBRARY)

      # Create an imported target for the component
      add_library(Ice::${component} UNKNOWN IMPORTED)
      set_target_properties(Ice::${component} PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Ice_INCLUDE_DIRS}"
      )

      if(Ice_${component}_LIBRARY_RELEASE)
        set_property(TARGET Ice::${component} APPEND PROPERTY
          IMPORTED_CONFIGURATIONS RELEASE
        )
        set_target_properties(Ice::${component} PROPERTIES
          IMPORTED_LOCATION_RELEASE "${Ice_${component}_LIBRARY_RELEASE}"
        )
      endif()

      if(Ice_${component}_LIBRARY_DEBUG)
        set_property(TARGET Ice::${component} APPEND PROPERTY
          IMPORTED_CONFIGURATIONS DEBUG
        )
        set_target_properties(Ice::${component} PROPERTIES
          IMPORTED_LOCATION_DEBUG "${Ice_${component}_LIBRARY_DEBUG}"
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
  endforeach()
endif()

unset(ice_include_path_suffixes)
unset(ice_bin_path_suffixes)
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
