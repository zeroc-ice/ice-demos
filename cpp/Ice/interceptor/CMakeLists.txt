cmake_minimum_required(VERSION 3.21)

project(interceptor CXX)

include(../../cmake/common.cmake)

add_executable(client Client.cpp Interceptor.ice)
slice2cpp_generate(client)
target_link_libraries(client Ice::Ice)

add_executable(server
    Server.cpp
    InterceptorI.cpp InterceptorI.h Interceptor.ice
    ThermostatI.cpp ThermostatI.h
    AuthenticatorI.cpp AuthenticatorI.h
)

slice2cpp_generate(server)
target_link_libraries(server Ice::Ice)
