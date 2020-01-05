# Support version 3.9 and above, but use policy settings up to 3.14.
# 3.9 is needed for project description.
cmake_minimum_required(VERSION 3.9...3.14)
# Ranges are supported from 3.12, set policy to current for < 3.12.
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Global build options.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build.")
option(BUILD_SHARED_LIBS "Build shared libraries." YES)

project(mastodonpp
  VERSION 0.0.0
  DESCRIPTION "C++ wrapper for the Mastodon API."
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Project build options.
option(WITH_TESTS "Compile tests." NO)
option(WITH_EXAMPLES "Compile examples." NO)
# option(WITH_DEB "Prepare for the building of .deb packages." NO)
# option(WITH_RPM "Prepare for the building of .rpm packages." NO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(debug_flags)

# Disable debug log.
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_definitions("-DNDEBUG")
endif()

add_subdirectory(src)
add_subdirectory(include)

add_subdirectory(cmake)
add_subdirectory(pkg-config)

if(WITH_TESTS)
  add_subdirectory(tests)
endif()

if(WITH_EXAMPLES)
  add_subdirectory(examples)
endif()

# include(cmake/packages.cmake)