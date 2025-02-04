cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

if (NOT Ice_HOME)
    if (DEFINED ENV{ICE_HOME})
        set(Ice_HOME $ENV{ICE_HOME})
    else()
        message(FATAL_ERROR "Ice_HOME not set")
    endif()
endif()

if (NOT EXISTS ${Ice_HOME})
    message(FATAL_ERROR "The specified Ice_HOME directory does not exist: ${Ice_HOME}")
endif()

find_program(Ice_SLICE2CPP_EXECUTABLE slice2cpp HINTS ${Ice_HOME}/cpp/bin PATH_SUFFIXES x64/Release x64/Debug)

if (NOT Ice_SLICE2CPP_EXECUTABLE)
    message(FATAL_ERROR "slice2cpp executable not found")
endif()

if (NOT DEFINED Ice_SLICE_DIR)
    set(Ice_SLICE_DIR ${Ice_HOME}/slice CACHE PATH "Path to Ice slice files")
endif()

# TODO Use a variable
if (WIN32)
set(Ice_INCLUDE_DIRS ${Ice_HOME}/cpp/include ${Ice_HOME}/cpp/include/generated ${Ice_HOME}/cpp/include/generated/x64/Release)
find_library(Ice_LIBRARY NAMES ice38a0 HINTS ${Ice_HOME}/cpp/lib PATH_SUFFIXES x64/Release)
# set(Ice_LIBRARY ${ICE_LIBRARY} PARENT_SCOPE)
elseif(APPLE)
set(Ice_INCLUDE_DIRS ${Ice_HOME}/cpp/include ${Ice_HOME}/cpp/include/generated)
set(Ice_LIBRARY ${Ice_HOME}/cpp/lib/libIce.dylib)
# find_library(Ice_LIBRARY NAMES Ice.dylib HINTS ${Ice_HOME}/cpp/lib/)
# set(Ice_LIBRARY ${ICE_LIBRARY} PARENT_SCOPE)
else()
set(Ice_INCLUDE_DIRS ${Ice_HOME}/cpp/include ${Ice_HOME}/cpp/include/generated)
set(Ice_LIBRARY ${Ice_HOME}/cpp/lib/libIce.so)
endif()

add_library(Ice::Ice SHARED IMPORTED)
set_target_properties(Ice::Ice PROPERTIES IMPORTED_IMPLIB ${Ice_LIBRARY})
set_target_properties(Ice::Ice PROPERTIES
    IMPORTED_LOCATION ${Ice_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES "${Ice_INCLUDE_DIRS}"
)

# Function to generate C++ source files from Slice (.ice) files for a target using slice2cpp
# The target must have the Slice files in its sources
# The generated files are added to the target sources
# Usage:
# add_executable(a_target source1.cpp source2.ice source3.ice)
# slice2cpp_generate(a_target)
function(slice2cpp_generate TARGET)

    # Get the list of source files for the target
    get_target_property(sources ${TARGET} SOURCES)

    # Create a directory to store the generated files
    set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/generated/${TARGET})
    make_directory(${output_dir})

    # Add the generated headers files to the target include directories
    target_include_directories(${TARGET} PRIVATE ${output_dir})

    # Process each Slice (.ice) file in the source list
    # 1. Run the slice2cpp command to generate the header and source files
    # 2. Add the generated files to the target sources
    foreach(file IN LISTS SOURCES)
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

            target_sources(${TARGET} PRIVATE ${output_files})

        endif()
    endforeach()

endfunction()
