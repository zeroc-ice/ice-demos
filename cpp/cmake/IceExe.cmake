# Find all executables in the native/bin directory and create imported targets
if(WIN32)
  # Create a helper function to add executable targets
  function(add_ice_executable name)
    add_executable(Ice::${name}_EXE IMPORTED)

    find_program(Ice_${name}_EXE_RELEASE ${name}${CMAKE_EXECUTABLE_SUFFIX}
      HINTS "${Ice_PREFIX}/build/native/bin/${Ice_WIN32_PLATFORM}/Release"
      NO_DEFAULT_PATH
    )

    find_program(Ice_${name}_EXE_DEBUG ${name}${CMAKE_EXECUTABLE_SUFFIX}
      HINTS "${Ice_PREFIX}/build/native/bin/${Ice_WIN32_PLATFORM}/Debug"
      NO_DEFAULT_PATH
    )

    if(Ice_${name}_EXE_RELEASE)
      set_property(TARGET Ice::${name}_EXE PROPERTY
        IMPORTED_LOCATION_RELEASE "${Ice_${name}_EXE_RELEASE}")
    endif()

    if(Ice_${name}_EXE_DEBUG)
      set_property(TARGET Ice::${name}_EXE PROPERTY
        IMPORTED_LOCATION_DEBUG "${Ice_${name}_EXE_DEBUG}")
    endif()

    # Set the appropriate location based on build type
    if(Ice_${name}_EXE_RELEASE OR Ice_${name}_EXE_DEBUG)
      set_property(TARGET Ice::${name}_EXE PROPERTY
        IMPORTED_LOCATION "$<IF:$<CONFIG:Debug>,${Ice_${name}_EXE_DEBUG},${Ice_${name}_EXE_RELEASE}>")
    endif()
  endfunction()

  add_ice_executable(icebox)
endif()
