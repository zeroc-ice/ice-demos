cmake_minimum_required(VERSION 3.21)

project(middleware CXX)

include(../../cmake/common.cmake)

# Client executable
add_executable(client ClientRun.cpp ClientRun.h Greeter.ice)

if(APPLE)
  target_sources(client PRIVATE ClientSecureTransport.cpp UtilSecureTransport.cpp)
elseif(WIN32)
  target_sources(client PRIVATE ClientSchannel.cpp UtilSchannel.cpp)
else()
  target_sources(client PRIVATE ClientOpenSSL.cpp)
endif()

slice2cpp_generate(client)
target_link_libraries(client PRIVATE Ice::Ice)
add_custom_command(TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:client>
  $<TARGET_RUNTIME_DLLS:client>
  $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

if(APPLE)
  target_link_libraries(client PRIVATE "-framework Security" "-framework CoreFoundation")
elseif(WIN32)
  target_link_libraries(client PRIVATE secur32.lib crypt32.lib)
else()
  target_link_libraries(client PRIVATE -lssl -lcrypto)
endif()

# Server executable
add_executable(server ServerRun.cpp ServerRun.h Chatbot.cpp Chatbot.h Greeter.ice)

if(APPLE)
  target_sources(server PRIVATE ServerSecureTransport.cpp UtilSecureTransport.cpp)
elseif(WIN32)
  target_sources(server PRIVATE ServerSchannel.cpp UtilSchannel.cpp)
else()
  target_sources(server PRIVATE ServerOpenSSL.cpp)
endif()

slice2cpp_generate(server)
target_link_libraries(server PRIVATE Ice::Ice)
add_custom_command(TARGET server POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:server>
  $<TARGET_RUNTIME_DLLS:server>
  $<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>
  COMMAND_EXPAND_LISTS
)

if(APPLE)
  target_link_libraries(server PRIVATE "-framework Security" "-framework CoreFoundation")
elseif(WIN32)
  target_link_libraries(server PRIVATE secur32.lib crypt32.lib)
else()
  target_link_libraries(server PRIVATE -lssl -lcrypto)
endif()
