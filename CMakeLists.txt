## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

## == Base project configuration
cmake_minimum_required(VERSION 3.0)
set(prj_NAME pujOgre)
set(prj_MAJ 1)
set(prj_MIN 0)
set(prj_REL 0)

## == Project definiton
project(${prj_NAME} VERSION "${prj_MAJ}.${prj_MIN}.${prj_REL}")

## == If working on a MacOSX, activate the use of RPATH's
## == Furthermore: prepare the type of executables
set(APP_TYPE "" CACHE STRING "Executable linking." FORCE)
if(APPLE)
  set(APP_TYPE "MACOSX_BUNDLE" CACHE STRING "Executable linking." FORCE)
  set(CMAKE_MACOSX_RPATH true CACHE BOOL "Use RPATH's on MacOSX." FORCE)
  mark_as_advanced(CMAKE_MACOSX_RPATH)
elseif(WIN32)
  set(APP_TYPE "WIN32" CACHE STRING "Executable linking." FORCE)
endif(APPLE)
mark_as_advanced(APP_TYPE)

## == Force c++11
set(CMAKE_CXX_STANDARD 11)

## == Prepare header generator to build shared libs
include(GenerateExportHeader)

## == Do not allow to build inside the source tree
if(PROJECT_BINARY_DIR STREQUAL ${PROJECT_SOURCE_DIR})
  message(FATAL_ERROR "Building in the source tree is not allowed.")
endif(PROJECT_BINARY_DIR STREQUAL ${PROJECT_SOURCE_DIR})

## == Where to put targets (executables and libs)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
mark_as_advanced(
  CMAKE_BACKWARDS_COMPATIBILITY
  EXECUTABLE_OUTPUT_PATH
  LIBRARY_OUTPUT_PATH
  )

## == Find Ogre
find_package(OGRE REQUIRED)

## == Find OIS
find_path(
  OIS_INCLUDE_DIRS
  NAMES OISConfig.h
  )
find_library(
  OIS_LIBRARIES
  NAMES OIS
  )

## == Build
subdirs(lib src)

## eof - $RCSfile$
