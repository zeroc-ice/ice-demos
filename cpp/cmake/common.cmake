cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

set(Ice_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path to Ice CMake configuration file")

# IceBT is an optional component as it is not available in all distributions.
find_package(Ice REQUIRED CONFIG COMPONENTS Ice DataStorm Glacier2 IceGrid IceBox IceStorm OPTIONAL_COMPONENTS IceBT)
