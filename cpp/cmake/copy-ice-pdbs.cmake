# Copyright (c) ZeroC, Inc.

# Build-time script: copy Ice PDB files that match the target's runtime DLLs.
#
# Required variables:
#   ICE_PDB_SOURCE_DIR - Directory containing PDB files for the matching arch/config
#   TARGET_DIR         - Target output directory
#   RUNTIME_DLLS       - Semicolon-separated list of runtime DLL paths

cmake_minimum_required(VERSION 3.21)

if(NOT EXISTS "${ICE_PDB_SOURCE_DIR}")
  return()
endif()

foreach(_dll_path IN LISTS RUNTIME_DLLS)
  if(NOT _dll_path)
    continue()
  endif()
  get_filename_component(_dll_name "${_dll_path}" NAME_WE)
  set(_pdb_file "${ICE_PDB_SOURCE_DIR}/${_dll_name}.pdb")
  if(EXISTS "${_pdb_file}")
    message(STATUS "Copying ${_dll_name}.pdb to ${TARGET_DIR}")
    file(COPY "${_pdb_file}" DESTINATION "${TARGET_DIR}")
  endif()
endforeach()
