# Support version 3.9 and above, but use policy settings up to 3.14.
# 3.9 is needed for project description.
cmake_minimum_required(VERSION 3.9...3.16)
# Ranges are supported from 3.12, set policy to current for < 3.12.
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Global build options.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build.")
option(BUILD_SHARED_LIBS "Build shared libraries." YES)

# Not every non-debug build type adds -DNDEBUG.
if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
  if(${CMAKE_VERSION} VERSION_LESS 3.12)
    add_definitions("-DNDEBUG")
  else()
    add_compile_definitions("NDEBUG")
  endif()
endif()

project(mastodonpp
  VERSION 0.5.7
  DESCRIPTION "C++ wrapper for the Mastodon and Pleroma APIs."
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Project build options.
option(WITH_TESTS "Compile tests." NO)
option(WITH_EXAMPLES "Compile examples." NO)
option(WITH_DOC "Generate API documentation." NO)
option(WITH_DEB "Prepare for the building of .deb packages." NO)
option(WITH_RPM "Prepare for the building of .rpm packages." NO)
option(WITH_CLANG-TIDY "Check sourcecode with clang-tidy while compiling." NO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(debug_flags)

if(WITH_CLANG-TIDY)
  set(CMAKE_CXX_CLANG_TIDY
    "clang-tidy"
    "-header-filter=${PROJECT_SOURCE_DIR}"
    "-quiet")
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

if(WITH_DOC)
  include(cmake/Doxygen.cmake)
  enable_doxygen(
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

include(cmake/packages.cmake)
