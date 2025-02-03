cmake_minimum_required(VERSION 3.16)

project(IceGreeterAsync CXX)

include(../../cmake/common.cmake)

get_filename_component(greeter_slice "Greeter.ice" ABSOLUTE)

set(slice_output_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
make_directory(${slice_output_dir})

set(greeter_slice_source "${slice_output_dir}/Greeter.cpp")
set(greeter_slice_header "${slice_output_dir}/Greeter.h")

add_custom_command(
    OUTPUT "${greeter_slice_source}" "${greeter_slice_header}"
    COMMAND ${Ice_SLICE2CPP_EXECUTABLE} --output-dir "${slice_output_dir}" "${greeter_slice}"
    DEPENDS "${greeter_slice}"
    COMMENT "Compiling ${greeter_slice}"
)

add_executable(ice_greeter_async_client Client.cpp ${greeter_slice_source} ${greeter_slice_header})
target_link_libraries(ice_greeter_async_client Ice::Ice)
target_include_directories(ice_greeter_async_client PRIVATE ${slice_output_dir})
set_target_properties(ice_greeter_async_client PROPERTIES OUTPUT_NAME client)

add_executable(ice_greeter_async_server Server.cpp ChatBot.cpp ChatBot.h ${greeter_slice_source} ${greeter_slice_header})
target_link_libraries(ice_greeter_async_server Ice::Ice)
target_include_directories(ice_greeter_async_server PRIVATE ${slice_output_dir})
set_target_properties(ice_greeter_async_server PROPERTIES OUTPUT_NAME server)
