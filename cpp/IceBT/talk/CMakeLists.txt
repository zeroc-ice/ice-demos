cmake_minimum_required(VERSION 3.21)

project(icegrid_allocate CXX)

include(../../cmake/common.cmake)

if(NOT TARGET Ice::IceBT)
    message(FATAL_ERROR "IceBT is not available")
    return()
endif()

add_executable(app App.cpp Talk.ice)
slice2cpp_generate(app)
target_link_libraries(app Ice::Ice Ice::IceBT)
