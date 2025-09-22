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

  # Find all executables in the bin directories
  file(GLOB ICE_EXECUTABLES
       "${Ice_PREFIX}/build/native/bin/${Ice_WIN32_PLATFORM}/Release/*.exe"
       "${Ice_PREFIX}/build/native/bin/${Ice_WIN32_PLATFORM}/Debug/*.exe")

  # Get unique executable names without path and extension
  set(ICE_EXE_NAMES)
  foreach(exe_path ${ICE_EXECUTABLES})
    get_filename_component(exe_name ${exe_path} NAME_WE)
    list(APPEND ICE_EXE_NAMES ${exe_name})
  endforeach()

  # Remove duplicates (debug/release variants)
  list(REMOVE_DUPLICATES ICE_EXE_NAMES)

  # Create imported targets for each executable
  foreach(exe ${ICE_EXE_NAMES})
    add_ice_executable(${exe})
  endforeach()
endif()
