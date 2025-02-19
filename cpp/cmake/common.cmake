cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32 AND NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/packages)
    message(STATUS "Downloading Ice NuGet package")
    # option(ICE_NUGET_INSTALL "Use NuGet to install Ice during configuration" ON)
    set(ICE_NUGET_SOURCE "https://download.zeroc.com/nexus/repository/nuget-nightly/" CACHE STRING "Ice NuGet package source")
    # if (ICE_NUGET_INSTALL)
    execute_process(
        COMMAND nuget install -Source ${ICE_NUGET_SOURCE} -OutputDirectory ${CMAKE_CURRENT_BINARY_DIR}/packages zeroc.ice.v143 -Prerelease -ExcludeVersion
        RESULT_VARIABLE nuget_result
        OUTPUT_VARIABLE nuget_output
        ERROR_VARIABLE nuget_error)
    if (nuget_result)
        message(FATAL_ERROR "Failed to download Ice NuGet package: ${nuget_error}")
    endif()
    # endif()

    set(Ice_HOME "${CMAKE_CURRENT_BINARY_DIR}/packages/zeroc.ice.v143" CACHE PATH "Path to Ice installation directory")
endif()

find_package(Threads REQUIRED)

set(Ice_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path to Ice CMake configuration file")

# IceBT is an optional component as it is not available in all distributions.
find_package(Ice REQUIRED CONFIG COMPONENTS Ice DataStorm Glacier2 IceGrid IceBox IceStorm OPTIONAL_COMPONENTS IceBT)
