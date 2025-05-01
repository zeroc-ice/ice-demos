cmake_minimum_required(VERSION 3.21)

project(icestorm_weather CXX)

include(../../cmake/common.cmake)

add_executable(sensor Sensor.cpp WeatherStation.ice)
slice2cpp_generate(sensor)
target_link_libraries(sensor PRIVATE Ice::Ice Ice::IceStorm)
add_custom_command(TARGET sensor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:sensor>
    $<TARGET_RUNTIME_DLLS:sensor>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

add_executable(station Station.cpp ConsolePrinter.cpp ConsolePrinter.h WeatherStation.ice)
slice2cpp_generate(station)
target_link_libraries(station PRIVATE Ice::Ice Ice::IceStorm)
add_custom_command(TARGET station POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:station>
    $<TARGET_RUNTIME_DLLS:station>
    $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)
