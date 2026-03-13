# Copyright (c) ZeroC, Inc.

# Ice PDB support for Windows.
# Downloads Ice PDB files at configure time and provides a function to copy them at build time.

set(ICE_PDB_DIR "${CMAKE_CURRENT_LIST_DIR}/pdb" CACHE PATH "Directory where Ice PDB files are extracted")
set(ICE_PDB_URL "https://download.zeroc.com/ice/3.8/windows-indexed-pdbs-${Ice_NUGET_VERSION}.zip" CACHE STRING "URL to download Ice PDB ZIP archive")

option(ICE_COPY_PDB "Download and copy Ice PDB files alongside DLLs" OFF)
if(ICE_COPY_PDB)
  if(NOT EXISTS "${ICE_PDB_DIR}")
    # Download and extract into a temp directory so ICE_PDB_DIR only exists after successful completion.
    set(_ice_pdb_temp_dir "${CMAKE_CURRENT_LIST_DIR}/.pdb-temp")
    file(REMOVE_RECURSE "${_ice_pdb_temp_dir}")
    file(MAKE_DIRECTORY "${_ice_pdb_temp_dir}")

    set(_ice_pdb_zip "${_ice_pdb_temp_dir}/ice-pdbs.zip")

    message(STATUS "Downloading Ice PDB archive from ${ICE_PDB_URL}")
    file(DOWNLOAD "${ICE_PDB_URL}" "${_ice_pdb_zip}"
      SHOW_PROGRESS
      STATUS _ice_pdb_download_status
      LOG _ice_pdb_download_log)

    list(GET _ice_pdb_download_status 0 _ice_pdb_status_code)
    if(_ice_pdb_status_code)
      file(REMOVE_RECURSE "${_ice_pdb_temp_dir}")
      list(GET _ice_pdb_download_status 1 _ice_pdb_error_message)
      message(FATAL_ERROR
        "Failed to download Ice PDB archive: ${_ice_pdb_error_message}\n"
        "URL: ${ICE_PDB_URL}\n"
        "Download log:\n${_ice_pdb_download_log}")
    endif()

    # The archive contains windows-symbols-sources/pdbs/ICE_VERSION/{platform}/{config}/*.pdb.
    # Extract only the pdbs subtree, then promote the version directory to ICE_PDB_DIR.
    message(STATUS "Extracting Ice PDB archive")
    file(ARCHIVE_EXTRACT INPUT "${_ice_pdb_zip}" DESTINATION "${_ice_pdb_temp_dir}" PATTERNS "*/pdbs/*")

    # Find the version directory (e.g. .pdb-temp/windows-symbols-sources/pdbs/3.8.0).
    file(GLOB _ice_pdb_version_dirs "${_ice_pdb_temp_dir}/*/pdbs/*")
    list(LENGTH _ice_pdb_version_dirs _ice_pdb_count)
    if(_ice_pdb_count EQUAL 0)
      file(REMOVE_RECURSE "${_ice_pdb_temp_dir}")
      message(FATAL_ERROR "No version directory found under pdbs/ in the Ice PDB archive")
    endif()
    list(GET _ice_pdb_version_dirs 0 _ice_pdb_version_dir)

    # Move the version directory to become ICE_PDB_DIR.
    file(RENAME "${_ice_pdb_version_dir}" "${ICE_PDB_DIR}")

    file(REMOVE_RECURSE "${_ice_pdb_temp_dir}")
    message(STATUS "Ice PDB files extracted to ${ICE_PDB_DIR}")
  endif()
endif()

# Copy Ice PDB files that match the DLLs in the target's output directory.
# PDBs are selected based on architecture and Debug/Release configuration.
# This is a no-op when ICE_COPY_PDB is OFF or on non-Windows platforms.
function(ice_copy_pdbs target)
  if(WIN32 AND ICE_COPY_PDB AND ICE_PDB_DIR)
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND "${CMAKE_COMMAND}"
        -DICE_PDB_SOURCE_DIR="${ICE_PDB_DIR}/${Ice_WIN32_PLATFORM}/$<IF:$<CONFIG:Debug>,Debug,Release>"
        -DTARGET_DIR="$<TARGET_FILE_DIR:${target}>"
        "-DRUNTIME_DLLS=$<TARGET_RUNTIME_DLLS:${target}>;$<GENEX_EVAL:$<TARGET_PROPERTY:Ice::Ice,ICE_RUNTIME_DLLS>>"
        -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/copy-ice-pdbs.cmake"
    )
  endif()
endfunction()
