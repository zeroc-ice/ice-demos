cmake_minimum_required(VERSION 3.21)

project(glacier2_session CXX)

include(../../cmake/common.cmake)

add_executable(client PokeBox.ice Client.cpp)

slice2cpp_generate(client)
target_link_libraries(client PRIVATE Ice::Ice Ice::Glacier2)
if(WIN32)
  add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client>
      $<TARGET_RUNTIME_DLLS:client>
      $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
    COMMAND_EXPAND_LISTS
  )
endif()

add_executable(
  server
  DefaultPokeSession.cpp
  DefaultPokeSession.h
  InMemoryPokeStore.cpp
  InMemoryPokeStore.h
  PokeBox.ice
  PokeStore.h
  Server.cpp
  SessionManager.cpp
  SessionManager.h
  SharedPokeBox.cpp
  SharedPokeBox.h
  UserIdResolver.h)

slice2cpp_generate(server)
target_link_libraries(server PRIVATE Ice::Ice Ice::Glacier2)
if(WIN32)
  add_custom_command(TARGET server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:server>
      $<TARGET_RUNTIME_DLLS:server>
      $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
    COMMAND_EXPAND_LISTS
  )
endif()
