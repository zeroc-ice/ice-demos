set(Ice_VERSION 3.8.0)

# set(Ice_HOME C:/Users/joe/Developer/zeroc-ice/ice)

if (NOT Ice_HOME)
    set(Ice_HOME $ENV{ICE_HOME} CACHE PATH "Path to Ice installation")
endif()

if (NOT Ice_HOME)
    # try to find Ice in an "ice" directory relative to the ice-demo directory
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../ice)
        set(Ice_HOME ${CMAKE_CURRENT_SOURCE_DIR}/../../ice)
    endif()
endif()

if (NOT Ice_HOME)
    message(FATAL_ERROR "Ice_HOME not set")
endif()

if (NOT EXISTS ${Ice_HOME})
    message(FATAL_ERROR "The specified Ice_HOME directory does not exist: ${Ice_HOME}")
endif()

find_program(Ice_SLICE2CPP_EXECUTABLE slice2cpp HINTS ${Ice_HOME}/cpp/bin PATH_SUFFIXES x64/Release x64/Debug)

if (NOT Ice_SLICE2CPP_EXECUTABLE)
    message(FATAL_ERROR "slice2cpp executable not found")
endif()

set(Ice_SLICE_DIR $(Ice_HOME)/slice CACHE PATH "Path to Ice slice files")

# TODO Use a variable
if (WIN32)
set(Ice_INCLUDE_DIRS ${Ice_HOME}/cpp/include ${Ice_HOME}/cpp/include/generated ${Ice_HOME}/cpp/include/generated/x64/Release)
find_library(Ice_LIBRARY NAMES ice38a0 HINTS ${Ice_HOME}/cpp/lib PATH_SUFFIXES x64/Release)
set(Ice_LIBRARY ${ICE_LIBRARY} PARENT_SCOPE)
elseif(APPLE)
set(Ice_INCLUDE_DIRS ${Ice_HOME}/cpp/include ${Ice_HOME}/cpp/include/generated)
set(Ice_LIBRARY ${Ice_HOME}/cpp/lib/libIce.dylib)
else()
set(Ice_INCLUDE_DIRS ${Ice_HOME}/cpp/include ${Ice_HOME}/cpp/include/generated)
set(Ice_LIBRARY ${Ice_HOME}/cpp/lib/libIce.so)
endif()

message(DEBUG "Ice_LIBRARY: ${Ice_LIBRARY}")

add_library(Ice::Ice SHARED IMPORTED)
set_target_properties(Ice::Ice PROPERTIES IMPORTED_IMPLIB ${Ice_LIBRARY})
set_target_properties(Ice::Ice PROPERTIES
    IMPORTED_LOCATION ${Ice_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES "${Ice_INCLUDE_DIRS}"
)

function(slice2cpp TARGET SLICE_FILE)
    get_filename_component(SLICE_NAME ${SLICE_FILE} NAME_WE)
    get_filename_component(SLICE_FILE_ABS ${SLICE_FILE} ABSOLUTE)

    set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/generated/${TARGET})
    set(output_files ${output_dir}/${SLICE_NAME}.h ${output_dir}/${SLICE_NAME}.cpp)

    make_directory(${output_dir})

    add_custom_command(
        OUTPUT ${output_files}
        COMMAND ${Ice_SLICE2CPP_EXECUTABLE} -I${Ice_SLICE_DIR} ${SLICE_FILE_ABS} --output-dir ${output_dir}
        DEPENDS ${SLICE_FILE_ABS}
        COMMENT "${Ice_SLICE2CPP_EXECUTABLE} ${SLICE_FILE_ABS} -> ${SLICE_NAME}.h ${SLICE_NAME}.cpp"
    )
    target_include_directories(${TARGET} PRIVATE ${output_dir})
    target_sources(${TARGET} PRIVATE ${output_files})
endfunction()
