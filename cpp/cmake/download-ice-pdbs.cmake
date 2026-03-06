# Copyright (c) ZeroC, Inc.

# Download and extract the Ice PDB archive.
# This script is included by common.cmake when ICE_USE_PDB is ON.
#
# Required variables:
#   ICE_PDB_URL - URL to the Ice PDB ZIP archive
#   ICE_PDB_DIR - Directory where PDB files will be extracted

cmake_minimum_required(VERSION 3.21)

if(NOT WIN32)
  message(FATAL_ERROR "download-ice-pdbs.cmake can only run on Windows")
endif()

if(NOT DEFINED ICE_PDB_URL OR ICE_PDB_URL STREQUAL "")
  message(FATAL_ERROR "ICE_PDB_URL must be set to the URL of the Ice PDB ZIP archive")
endif()

if(NOT DEFINED ICE_PDB_DIR OR ICE_PDB_DIR STREQUAL "")
  message(FATAL_ERROR "ICE_PDB_DIR must be set")
endif()

set(ICE_PDB_ZIP "${ICE_PDB_DIR}/ice-pdbs.zip")

message(STATUS "Downloading Ice PDB archive from ${ICE_PDB_URL}")
file(DOWNLOAD "${ICE_PDB_URL}" "${ICE_PDB_ZIP}"
  SHOW_PROGRESS
  STATUS download_status
  LOG download_log)

list(GET download_status 0 status_code)
if(status_code)
  list(GET download_status 1 error_message)
  message(FATAL_ERROR
    "Failed to download Ice PDB archive: ${error_message}\n"
    "URL: ${ICE_PDB_URL}\n"
    "Download log:\n${download_log}")
endif()

# The archive contains windows-symbols-sources/pdbs/ICE_VERSION/{platform}/{config}/*.pdb.
# We extract only the pdbs subtree and flatten so ICE_PDB_DIR contains {platform}/{config}/*.pdb directly.
set(_ice_pdb_extract_dir "${ICE_PDB_DIR}/_extract")

message(STATUS "Extracting Ice PDB archive to ${ICE_PDB_DIR}")
file(ARCHIVE_EXTRACT INPUT "${ICE_PDB_ZIP}" DESTINATION "${_ice_pdb_extract_dir}" PATTERNS "*/pdbs/*")
file(REMOVE "${ICE_PDB_ZIP}")

# Find the version directory inside the extracted pdbs folder (e.g. _extract/windows-symbols-sources/pdbs/3.8.0).
file(GLOB _ice_pdb_version_dirs "${_ice_pdb_extract_dir}/*/pdbs/*")
list(LENGTH _ice_pdb_version_dirs _ice_pdb_count)
if(_ice_pdb_count EQUAL 0)
  message(FATAL_ERROR "No version directory found under pdbs/ in the Ice PDB archive")
endif()
list(GET _ice_pdb_version_dirs 0 _ice_pdb_version_dir)

# Move the contents of the version directory into ICE_PDB_DIR.
file(GLOB _ice_pdb_contents "${_ice_pdb_version_dir}/*")
foreach(_item IN LISTS _ice_pdb_contents)
  cmake_path(GET _item FILENAME _name)
  file(RENAME "${_item}" "${ICE_PDB_DIR}/${_name}")
endforeach()

file(REMOVE_RECURSE "${_ice_pdb_extract_dir}")

message(STATUS "Ice PDB download and extraction completed.")
