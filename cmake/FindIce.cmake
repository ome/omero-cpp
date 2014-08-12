#.rst:
# FindIce
# -------
#
# Find the ZeroC Internet Communication Engine (ICE) programs,
# libraries and datafiles.
#
# Use this module by invoking find_package with the form::
#
#   find_package(Ice
#     [version] [EXACT]       # Minimum or EXACT version e.g. 3.5.1
#     [REQUIRED]              # Fail with error if Ice is not found
#     [COMPONENTS <libs...>]) # Ice libraries by their name
#
# Components can include any of: Freeze Glacier2 Ice IceBox IceDB
# IceGrid IcePatch IceSSL IceStorm IceUtil IceXML Slice.
#
# This module reports information about the Ice installation in
# several variables.  General variables::
#
#   Ice_VERSION - Ice release version
#   Ice_FOUND - true if the main programs and libraries were found
#   ICE_LIBRARIES - component libraries to be linked
#   Ice_BINARY_DIR - the directory containing the Ice programs
#   Ice_INCLUDE_DIR - the directory containing the Ice headers
#   Ice_SLICE_DIR - the directory containing the Ice slice interface definitions
#   Ice_LIBRARY_DIR - the directory containing the Ice libraries
#
# Ice programs are reported in::
#
#   Ice_SLICE2CPP_EXECUTABLE - path to slice2cpp executable
#   Ice_SLICE2CS_EXECUTABLE - path to slice2cs executable
#   Ice_SLICE2FREEZEJ_EXECUTABLE - path to slice2freezej executable
#   Ice_SLICE2FREEZE_EXECUTABLE - path to slice2freeze executable
#   Ice_SLICE2HTML_EXECUTABLE - path to slice2html executable
#   Ice_SLICE2JAVA_EXECUTABLE - path to slice2java executable
#   Ice_SLICE2PHP_EXECUTABLE - path to slice2php executable
#   Ice_SLICE2PY_EXECUTABLE - path to slice2py executable
#   Ice_SLICE2RB_EXECUTABLE - path to slice2rb executable
#
# Ice component libraries are reported in::
#
#   Ice_<C>_FOUND - ON if component library was found
#   ICE_<C>_LIBRARY - component library
#
# This module reads hints about search results from variables::
#
#   ICE_HOME - the root of the Ice installation
#   ICE_BINARYDIR - the directory containing the Ice programs
#   ICE_INCLUDEDIR - the directory containing the Ice headers
#   ICE_SLICEDIR - the directory containing the Ice slice interface definitions
#   ICE_LIBRARYDIR - the directory containing the Ice libraries
#
# The environment variable :envvar:`ICE_HOME` may also be used, unless
# overridden by setting the ICE_HOME variable.
#
# .. note::
#
#   These variables are not all required to be set, and in most cases
#   will not require setting at all unless multiple Ice versions are
#   available and a specific version is required.  On Windows,
#   ICE_HOME is usually sufficient since the package is contained in a
#   single directory.  On Unix, the programs, headers and libraries
#   will usually be in standard locations, but Ice_SLICE_DIR might not
#   be automatically detected.  All the other variables are defaulted
#   using ICE_HOME, if set.  It's possible to set ICE_HOME and
#   selectively specify alternative locations for the other
#   components; this might be required for e.g. newer versions of
#   Visual Studio if the heuristics are not sufficient to identify the
#   correct programs and libraries.
#
# Other variables one may set to control this module are::
#
#   ICE_DEBUG - Set to ON to enable debug output from FindIce.

# Written by Roger Leigh <rleigh@codelibre.net>

#=============================================================================
# Copyright 2014 University of Dundee
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

set(ICE_HOME NOTFOUND
    CACHE PATH "Location of the Ice installation")
mark_as_advanced(FORCE ICE_HOME)

set(ICE_BINARYDIR NOTFOUND
    CACHE PATH "Location of the Ice programs")
mark_as_advanced(FORCE ICE_BINARYDIR)

set(ICE_INCLUDEDIR NOTFOUND
    CACHE PATH "Location of the Ice headers")
mark_as_advanced(FORCE ICE_INCLUDEDIR)

set(ICE_SLICEDIR NOTFOUND
    CACHE PATH "Location of the Ice slice interface definitions")
mark_as_advanced(FORCE ICE_SLICEDIR)

set(ICE_LIBRARYDIR NOTFOUND
    CACHE PATH "Location of the Ice libraries")
mark_as_advanced(FORCE ICE_LIBRARYDIR)

function(_Ice_FIND)
  # Released versions of Ice, including generic short forms
  set(ice_versions
      3
      3.5
      3.5.1
      3.5.0
      3.4
      3.4.2
      3.4.1
      3.4.0
      3.3
      3.3.1
      3.3.0)

  # Set up search paths, taking compiler into account.  Search ICE_HOME,
  # with ICE_HOME in the environment as a fallback
  if(ICE_HOME)
    list(APPEND ice_roots "${ICE_HOME}")
  endif(ICE_HOME)
  if(EXISTS "$ENV{ICE_HOME}")
    file(TO_CMAKE_PATH "$ENV{ICE_HOME}" NATIVE_PATH)
    list(APPEND ice_roots "${NATIVE_PATH}")
  endif(EXISTS "$ENV{ICE_HOME}")

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64-bit path suffix
    set(_x64 "/x64")
    # 64-bit library directory
    set(_lib64 "lib64")
  endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

  if(MSVC_VERSION)
    # VS 8.0
    if((MSVC_VERSION EQUAL 1400) OR (MSVC_VERSION GREATER 1400 AND MSVC_VERSION LESS 1500))
      set(vcver vc80)
    endif((MSVC_VERSION EQUAL 1400) OR (MSVC_VERSION GREATER 1400 AND MSVC_VERSION LESS 1500))
    # VS 9.0
    if((MSVC_VERSION EQUAL 1500) OR (MSVC_VERSION GREATER 1500 AND MSVC_VERSION LESS 1600))
      set(vcver vc90)
    endif((MSVC_VERSION EQUAL 1500) OR (MSVC_VERSION GREATER 1500 AND MSVC_VERSION LESS 1600))
    # VS 10.0
    if((MSVC_VERSION EQUAL 1600) OR (MSVC_VERSION GREATER 1600 AND MSVC_VERSION LESS 1700))
      set(vcver vc100)
    endif((MSVC_VERSION EQUAL 1600) OR (MSVC_VERSION GREATER 1600 AND MSVC_VERSION LESS 1700))
    # VS 11.0
    if((MSVC_VERSION EQUAL 1700) OR (MSVC_VERSION GREATER 1700 AND MSVC_VERSION LESS 1800))
      set(vcver vc110)
    endif((MSVC_VERSION EQUAL 1700) OR (MSVC_VERSION GREATER 1700 AND MSVC_VERSION LESS 1800))
    # VS 12.0
    if((MSVC_VERSION EQUAL 1800) OR (MSVC_VERSION GREATER 1800 AND MSVC_VERSION LESS 1900))
      set(vcver vc120)
    endif((MSVC_VERSION EQUAL 1800) OR (MSVC_VERSION GREATER 1800 AND MSVC_VERSION LESS 1900))
    # VS 14.0
    if((MSVC_VERSION EQUAL 1900) OR (MSVC_VERSION GREATER 1900 AND MSVC_VERSION LESS 2000))
      set(vcver vc140)
    endif((MSVC_VERSION EQUAL 1900) OR (MSVC_VERSION GREATER 1900 AND MSVC_VERSION LESS 2000))
  endif(MSVC_VERSION)

  foreach(root ${ice_roots})
    # For compatibility with ZeroC Windows builds.
    if(vcver)
      # Versions prior to VS 10.0 don't use vcnnn subdirectories, but are harmless to check.
      list(APPEND ice_binary_paths "${root}/bin/${vcver}${_x64}")
      list(APPEND ice_library_paths "${root}/lib/${vcver}${_x64}")
      list(APPEND ice_binary_paths "${root}/bin/${vcver}")
      list(APPEND ice_library_paths "${root}/lib/${vcver}")
    endif(vcver)
    # Generic 64-bit directories
    list(APPEND ice_binary_paths "${root}/bin${_x64}")
    list(APPEND ice_library_paths "${root}/${_lib64}")
    list(APPEND ice_library_paths "${root}/lib${_x64}")
    # Generic 64-bit or 32-bit directories
    list(APPEND ice_binary_paths "${root}/bin")
    list(APPEND ice_include_paths "${root}/include")
    # Common directories
    list(APPEND ice_library_paths "${root}/lib")
    list(APPEND ice_slice_paths "${root}/slice")
  endforeach(root ${ice_roots})

  # On Windows, look in standard install locations.  Different versions
  # of Ice install in different places and support different compiler
  # versions.  Look only in the locations compatible with the compiler
  # in use.  For newer versions which this hardcoded logic can't
  # support, ICE_HOME and/or the other configuration options must be
  # used, in which case the above logic will be used instead.
  if(vcver)
    set(_x86 "(x86)")
    if (CMAKE_SIZEOF_VOID_P MATCHES 8)
      set (program_files_path "$ENV{ProgramFiles${_x86}}/ZeroC")
    else (CMAKE_SIZEOF_VOID_P MATCHES 8)
      set (program_files_path "$ENV{ProgramFiles}/ZeroC")
    endif (CMAKE_SIZEOF_VOID_P MATCHES 8)
    file(TO_CMAKE_PATH "${program_files_path}" program_files_path)

    string(TOUPPER "${vcver}" VCVER)

    set(ice_locations
        ${program_files_path}
        "C:/")

    foreach(ice_version ${ice_versions})
      foreach(ice_location ${ice_locations})

        # Search for version-specific VS builds before generic location
        list(APPEND ice_binary_paths "${ice_location}/Ice-${ice_version}/bin/${vcver}${_x64}")
        list(APPEND ice_library_paths "${ice_location}/Ice-${ice_version}/lib/${vcver}${_x64}")
        list(APPEND ice_binary_paths "${ice_location}/Ice-${ice_version}/bin/${vcver}")
        list(APPEND ice_library_paths "${ice_location}/Ice-${ice_version}/lib/${vcver}")
        list(APPEND ice_binary_paths "${ice_location}/Ice-${ice_version}-${VCVER}/bin${_x64}")
        list(APPEND ice_library_paths "${ice_location}/Ice-${ice_version}-${VCVER}/lib${_x64}")
        list(APPEND ice_binary_paths "${ice_location}/Ice-${ice_version}-${VCVER}/bin")
        list(APPEND ice_library_paths "${ice_location}/Ice-${ice_version}-${VCVER}/lib")
        list(APPEND ice_binary_paths "${ice_location}/Ice-${ice_version}/bin${_x64}")
        list(APPEND ice_library_paths "${ice_location}/Ice-${ice_version}/lib${_x64}")
        list(APPEND ice_binary_paths "${ice_location}/Ice-${ice_version}/bin")
        list(APPEND ice_library_paths "${ice_location}/Ice-${ice_version}/lib")
        list(APPEND ice_include_paths "${ice_location}/Ice-${ice_version}/include")
        list(APPEND ice_slice_paths "${ice_location}/Ice-${ice_version}/slice")

      endforeach(ice_location)
    endforeach(ice_version)

  else(vcver)
    foreach(ice_version ${ice_versions})
      # Prefer 64-bit variants if present (and using a 64-bit compiler)
      list(APPEND ice_binary_paths  "/opt/Ice-${ice_version}/bin${_x64}")
      list(APPEND ice_binary_paths  "/opt/Ice-${ice_version}/bin")
      list(APPEND ice_library_paths "/opt/Ice-${ice_version}/lib${_x64}")
      list(APPEND ice_library_paths "/opt/Ice-${ice_version}/${_lib64}")
      list(APPEND ice_library_paths "/opt/Ice-${ice_version}/lib")
      list(APPEND ice_include_paths "/opt/Ice-${ice_version}/include")
      list(APPEND ice_slice_paths   "/opt/Ice-${ice_version}/slice")
    endforeach(ice_version)
  endif(vcver)

  set(ice_programs
      slice2cpp
      slice2cs
      slice2freezej
      slice2freeze
      slice2html
      slice2java
      slice2php
      slice2py
      slice2rb)

  set(ice_libraries
      Freeze
      Glacier2
      Ice
      IceBox
      IceDB
      IceGrid
      IcePatch2
      IceSSL
      IceStorm
      IceUtil
      IceXML
      Slice)

  # Find all Ice programs
  foreach(program ${ice_programs})
    string(TOUPPER "${program}" program_upcase)
    set(program_var "Ice_${program_upcase}_EXECUTABLE")
    find_program("${program_var}" "${program}"
      PATHS "${ICE_BINARYDIR}"
            ${ice_binary_paths}
      DOC "Ice slice translator")
    mark_as_advanced(program_var)
    set("${program_var}" "${${program_var}}" PARENT_SCOPE)
    if(NOT FOUND_ICE_BINARY_DIR)
      get_filename_component(FOUND_ICE_BINARY_DIR "${${program_var}}" PATH)
    endif(NOT FOUND_ICE_BINARY_DIR)
  endforeach(program ${ice_programs})
  set(Ice_BINARY_DIR "${FOUND_ICE_BINARY_DIR}" PARENT_SCOPE)

  # Get version.
  if(Ice_SLICE2CPP_EXECUTABLE)
    # Execute in C locale for safety
    set(_Ice_SAVED_LC_ALL "$ENV{LC_ALL}")
    set(ENV{LC_ALL} C)

    execute_process(COMMAND ${Ice_SLICE2CPP_EXECUTABLE} --version
      ERROR_VARIABLE Ice_VERSION_SLICE2CPP_FULL
      ERROR_STRIP_TRAILING_WHITESPACE)

    # restore the previous LC_ALL
    set(ENV{LC_ALL} ${_Ice_SAVED_LC_ALL})

    # Make short version
    string(REGEX REPLACE "^(.*)\\.[^.]*$" "\\1" Ice_VERSION_SLICE2CPP_SHORT "${Ice_VERSION_SLICE2CPP_FULL}")
    set(ICE_VERSION "${Ice_VERSION_SLICE2CPP_FULL}" PARENT_SCOPE)
  endif(Ice_SLICE2CPP_EXECUTABLE)

  # The following searches prefer the version found; note reverse
  # order due to prepending.
  if(NOT MSVC)
    list(INSERT ice_slice_paths   0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_SHORT}/slice")
    list(INSERT ice_include_paths 0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_SHORT}/include")
    list(INSERT ice_library_paths 0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_SHORT}/lib")
    list(INSERT ice_library_paths 0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_SHORT}/${_lib64}")
    list(INSERT ice_library_paths 0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_SHORT}/lib${_x64}")
    list(INSERT ice_binary_paths  0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_SHORT}/bin")
    list(INSERT ice_binary_paths  0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_SHORT}/bin${_x64}")

    list(INSERT ice_slice_paths   0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_FULL}/slice")
    list(INSERT ice_include_paths 0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_FULL}/include")
    list(INSERT ice_library_paths 0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_FULL}/lib")
    list(INSERT ice_library_paths 0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_FULL}/${_lib64}")
    list(INSERT ice_library_paths 0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_FULL}/lib${_x64}")
    list(INSERT ice_binary_paths  0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_FULL}/bin")
    list(INSERT ice_binary_paths  0 "/opt/Ice-${Ice_VERSION_SLICE2CPP_FULL}/bin${_x64}")
  endif(NOT MSVC)

  # Find include directory
  find_path(ICE_INCLUDE_DIR
            NAMES "Ice/Ice.h"
            PATHS  "${ICE_INCLUDEDIR}"
                   ${ice_include_paths})
  set(Ice_INCLUDE_DIR "${ICE_INCLUDE_DIR}" PARENT_SCOPE)

  # In common use on Linux and MacOS X (homebrew); prefer version-specific dir
  list(APPEND ice_slice_paths
    "/usr/local/share/Ice-${Ice_VERSION_SLICE2CPP_FULL}/slice"
    "/usr/local/share/Ice-${Ice_VERSION_SLICE2CPP_SHORT}/slice"
    "/usr/local/share/Ice/slice"
    "/usr/share/Ice-${Ice_VERSION_SLICE2CPP_FULL}/slice"
    "/usr/share/Ice-${Ice_VERSION_SLICE2CPP_SHORT}/slice"
    "/usr/share/Ice/slice")

  # Find slice directory
  find_path(ICE_SLICE_DIR
            NAMES "Ice/Connection.ice"
            PATHS "${ICE_SLICEDIR}"
                  ${ice_slice_paths}
            NO_DEFAULT_PATH)
  set(Ice_SLICE_DIR "${ICE_SLICE_DIR}" PARENT_SCOPE)

  # Find all Ice libraries
  foreach(component ${Ice_FIND_COMPONENTS})
    string(TOUPPER "${component}" component_upcase)
    set(component_lib "${component_upcase}_LIBRARY")
    set(component_found "${component_upcase}_FOUND")
    find_library("${component_lib}" "${component}"
      PATHS
        "${ICE_LIBRARYDIR}"
        ${ice_library_paths}
      HINT
        "${ICE_HOME}/lib"
      DOC "Ice slice translator")
    mark_as_advanced("${component_lib}")
    if("${component_lib}")
      set("${component_found}" ON)
      list(APPEND ICE_LIBRARIES "${${component_lib}}")
    endif("${component_lib}")
    mark_as_advanced("${component_found}")
    set("${component_found}" "${${component_found}}" PARENT_SCOPE)
    if(NOT FOUND_ICE_LIBRARY_DIR)
      get_filename_component(FOUND_ICE_LIBRARY_DIR "${${component_lib}}" PATH)
    endif(NOT FOUND_ICE_LIBRARY_DIR)
  endforeach(component)
  set(Ice_LIBRARIES "${ICE_LIBRARIES}" PARENT_SCOPE)
  set(Ice_LIBRARY_DIR "${FOUND_ICE_LIBRARY_DIR}" PARENT_SCOPE)

  if(ICE_DEBUG)
    message(STATUS "--------FindIce.cmake search debug--------")
    message(STATUS "ICE binary path search order: ${ice_binary_paths}")
    message(STATUS "ICE include path search order: ${ice_include_paths}")
    message(STATUS "ICE slice path search order: ${ice_slice_paths}")
    message(STATUS "ICE library path search order: ${ice_library_paths}")
    message(STATUS "----------------")
  endif(ICE_DEBUG)
endfunction(_Ice_FIND)

_Ice_FIND()

if(ICE_DEBUG)
  message(STATUS "--------FindIce.cmake results debug--------")
  message(STATUS "ICE_VERSION number: ${ICE_VERSION}")
  message(STATUS "ICE_HOME directory: ${ICE_HOME}")
  message(STATUS "Ice_BINARY_DIR directory: ${Ice_BINARY_DIR}")
  message(STATUS "Ice_INCLUDE_DIR directory: ${Ice_INCLUDE_DIR}")
  message(STATUS "Ice_SLICE_DIR directory: ${Ice_SLICE_DIR}")
  message(STATUS "Ice_LIBRARY_DIR directory: ${Ice_LIBRARY_DIR}")
  message(STATUS "Ice_LIBRARIES: ${Ice_LIBRARIES}")
  message(STATUS "slice2cpp executable: ${Ice_SLICE2CPP_EXECUTABLE}")
  message(STATUS "slice2cs executable: ${Ice_SLICE2CS_EXECUTABLE}")
  message(STATUS "slice2freezej executable: ${Ice_SLICE2FREEZEJ_EXECUTABLE}")
  message(STATUS "slice2freeze executable: ${Ice_SLICE2FREEZE_EXECUTABLE}")
  message(STATUS "slice2html executable: ${Ice_SLICE2HTML_EXECUTABLE}")
  message(STATUS "slice2java executable: ${Ice_SLICE2JAVA_EXECUTABLE}")
  message(STATUS "slice2php executable: ${Ice_SLICE2PHP_EXECUTABLE}")
  message(STATUS "slice2py executable: ${Ice_SLICE2PY_EXECUTABLE}")
  message(STATUS "slice2rb executable: ${Ice_SLICE2RB_EXECUTABLE}")
  foreach(component ${Ice_FIND_COMPONENTS})
    string(TOUPPER "${component}" component_upcase)
    set(component_lib "${component_upcase}_LIBRARY")
    set(component_found "${component_upcase}_FOUND")
    message(STATUS "${component} library found: ${${component_found}}")
    message(STATUS "${component} library: ${${component_lib}}")
  endforeach(component ${Ice_FIND_COMPONENTS})
  message(STATUS "----------------")
endif(ICE_DEBUG)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Ice
                                  REQUIRED_VARS Ice_SLICE2CPP_EXECUTABLE
                                                Ice_INCLUDE_DIR
                                                Ice_SLICE_DIR
                                                ICE_LIBRARY
                                  VERSION_VAR ICE_VERSION)

unset(ICE_VERSION)