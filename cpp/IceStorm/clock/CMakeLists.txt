cmake_minimum_required(VERSION 3.21)

project(icestorm_clock CXX)

include(../../cmake/common.cmake)

add_executable(subscriber Subscriber.cpp Clock.ice)
slice2cpp_generate(subscriber)
target_link_libraries(subscriber PRIVATE Ice::Ice Ice::IceStorm)
add_custom_command(TARGET subscriber POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:subscriber>
  $<TARGET_RUNTIME_DLLS:subscriber>
  $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

add_executable(publisher Publisher.cpp Clock.ice)
slice2cpp_generate(publisher)
target_link_libraries(publisher PRIVATE Ice::Ice Ice::IceStorm)
add_custom_command(TARGET publisher POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:publisher>
  $<TARGET_RUNTIME_DLLS:publisher>
  $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)
