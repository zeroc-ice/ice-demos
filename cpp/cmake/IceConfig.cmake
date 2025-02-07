if (NOT Ice_HOME)
  if (DEFINED ENV{ICE_HOME})
    set(Ice_HOME $ENV{ICE_HOME} CACHE PATH "Path to the Ice installation directory")
  else()
    message(FATAL_ERROR "Ice_HOME not set")
  endif()
endif()

if (NOT EXISTS ${Ice_HOME})
  message(FATAL_ERROR "The specified Ice_HOME directory does not exist: ${Ice_HOME}")
endif()

find_program(Ice_SLICE2CPP_EXECUTABLE slice2cpp HINTS ${Ice_HOME}/cpp/bin PATH_SUFFIXES x64/Release x64/Debug)

if(NOT DEFINED Ice_SLICE_DIR AND EXISTS ${Ice_HOME}/slice)
  set(Ice_SLICE_DIR ${Ice_HOME}/slice CACHE PATH "Path to the Ice Slice files directory")
endif()

# This is the only version we support for the demos right now
set(Ice_VERSION "3.8.0-alpha.0" CACHE STRING "Ice version")
set(Ice_DLL_VERSION "38a0" CACHE STRING "Ice DLL version")

find_path(Ice_INCLUDE_DIR NAMES Ice/Ice.h HINTS ${Ice_HOME}/cpp/include)

if (CMAKE_LIBRARY_ARCHITECTURE)
  list(APPEND lib_path_suffixes ${CMAKE_LIBRARY_ARCHITECTURE})
endif()

if(WIN32)
  list(APPEND lib_path_suffixes x64/Release)
endif()

find_library(Ice_LIBRARY NAMES Ice Ice${Ice_DLL_VERSION} HINTS ${Ice_HOME}/cpp/lib/ PATH_SUFFIXES ${lib_path_suffixes})

if (Ice_FIND_COMPONENTS)
  foreach(component IN LISTS Ice_FIND_COMPONENTS)
    string(TOUPPER "${component}" component_upcase)
    find_library(Ice_${component_upcase}_LIBRARY NAMES ${component} ${component}${Ice_DLL_VERSION} HINTS ${Ice_HOME}/cpp/lib/ PATH_SUFFIXES ${lib_path_suffixes})
  endforeach()
endif()

find_package_handle_standard_args(Ice
                  REQUIRED_VARS Ice_SLICE2CPP_EXECUTABLE
                        Ice_INCLUDE_DIR
                        Ice_LIBRARY
                        Ice_SLICE_DIR
                  VERSION_VAR Ice_VERSION)

if(Ice_FOUND)
  # set(Ice_LIBRARIES ${Ice_LIBRARY})
  if(WIN32)
    set(Ice_INCLUDE_DIRS ${Ice_INCLUDE_DIR} ${Ice_HOME}/cpp/include/generated ${Ice_HOME}/cpp/include/generated/x64/Release)
  else()
    set(Ice_INCLUDE_DIRS ${Ice_INCLUDE_DIR} ${Ice_HOME}/cpp/include/generated)
  endif()

  add_library(Ice::Ice UNKNOWN IMPORTED)
  # set_target_properties(Ice::Ice PROPERTIES IMPORTED_IMPLIB ${Ice_LIBRARY})
  set_target_properties(Ice::Ice PROPERTIES
    IMPORTED_LOCATION ${Ice_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES "${Ice_INCLUDE_DIRS}"
  )

  if (Ice_FIND_COMPONENTS)
    foreach(component IN LISTS Ice_FIND_COMPONENTS)
      string(TOUPPER "${component}" component_upcase)
      add_library(Ice::${component} UNKNOWN IMPORTED)
      set_target_properties(Ice::${component} PROPERTIES
        IMPORTED_LOCATION ${Ice_${component_upcase}_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES "${Ice_INCLUDE_DIRS}"
      )
    endforeach()
  endif()
endif()

# Function to generate C++ source files from Slice (.ice) files for a target using slice2cpp
# The target must have the Slice files in its sources
# The generated files are added to the target sources
# Usage:
# add_executable(a_target source1.cpp source2.ice source3.ice)
# slice2cpp_generate(a_target)
function(slice2cpp_generate target)

  # Get the list of source files for the target
  get_target_property(sources ${target} SOURCES)

  # Create a directory to store the generated files
  set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/generated/${target})
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
        COMMENT "${Ice_SLICE2CPP_EXECUTABLE} ${file} -> ${slice_file_name}.h ${slice_file_name}.cpp"
      )

      target_sources(${target} PRIVATE ${output_files})

    endif()
  endforeach()
endfunction()

if(Ice_DEBUG)
  message(STATUS "Ice_VERSION: ${Ice_VERSION}")
  message(STATUS "Ice_HOME: ${Ice_HOME}")
  message(STATUS "Ice_INCLUDE_DIR: ${Ice_INCLUDE_DIR}")
  message(STATUS "Ice_INCLUDE_DIRS: ${Ice_INCLUDE_DIRS}")
  message(STATUS "Ice_SLICE_DIR directory: ${Ice_SLICE_DIR}")
  message(STATUS "Ice_LIBRARY: ${Ice_LIBRARY}")
endif()
