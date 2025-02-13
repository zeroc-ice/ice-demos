cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CMAKE_COMPILE_WARNING_AS_ERROR "Compile warnings as errors" ON)

# We use these flags over presets to avoid having to create a CMakePreset.json file in every project.
# If you want to disable this behavior, set the option to OFF: -DUSE_ICE_CXX_FLAGS=OFF
option(USE_ICE_CXX_FLAGS "Common C++ compiler flags" ON)
if(USE_ICE_CXX_FLAGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(flags -Wall -Wall -Wextra -Wredundant-decls -Wshadow -Wdeprecated)
    elseif(flags STREQUAL "Clang")
        set(ICE_CXX_FLAGS -Wall -Wextra -Wshadow -Wredundant-decls -Wno-shadow-field -Wdeprecated -Wstrict-prototypes -Wconversion -Wdocumentation)
    elseif(flags STREQUAL "MSVC")
        set(ICE_CXX_FLAGS /W4)
    endif()
    add_compile_options(${flags})
    unset(flags)
endif()

find_package(Threads REQUIRED)

set(Ice_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path to Ice CMake configuration file")

# IceBT is an optional component as it is not available in all distributions.
find_package(Ice REQUIRED CONFIG COMPONENTS Ice DataStorm Glacier2 IceGrid IceBox IceStorm OPTIONAL_COMPONENTS IceBT)
