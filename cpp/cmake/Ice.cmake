set(Ice_VERSION 3.8.0)

set(Ice_HOME $ENV{ICE_HOME})
if (NOT Ice_HOME)
    message(FATAL_ERROR "ICE_HOME environment variable missing")
endif()

find_program(Ice_SLICE2CPP_EXECUTABLE slice2cpp HINTS ${Ice_HOME}/cpp/bin)

set(Ice_SLICE_DIR $(Ice_HOME)/slice)

set(Ice_INCLUDE_DIRS ${Ice_HOME}/cpp/include ${Ice_HOME}/cpp/include/generated)
# find_path(Ice_INCLUDE_DIR NAMES Ice/Ice.h HINTS ${Ice_HOME}/cpp/include)

find_library(Ice_LIBRARIES NAMES Ice HINTS ${Ice_HOME}/cpp/lib)

add_library(Ice::Ice SHARED IMPORTED)
set_target_properties(Ice::Ice PROPERTIES
    IMPORTED_LOCATION ${Ice_HOME}/cpp/lib/libIce.dylib
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
