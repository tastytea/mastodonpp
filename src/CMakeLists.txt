include(GNUInstallDirs)

find_package(CURL 7.56 REQUIRED)

# Write version in header.
configure_file ("version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.hpp" @ONLY)

add_library(${PROJECT_NAME})

file(GLOB_RECURSE sources_lib *.cpp)
file(GLOB_RECURSE headers_lib ../include/*.hpp)
target_sources(${PROJECT_NAME}
  PRIVATE "${sources_lib}" "${headers_lib}")

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

target_include_directories(${PROJECT_NAME}
  PRIVATE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>" # version.hpp
  PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# FindCURL provides an IMPORTED target since CMake 3.12.
if(NOT ${CMAKE_VERSION} VERSION_LESS 3.12)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC CURL::libcurl)
else()
  target_include_directories(${PROJECT_NAME}
    PUBLIC ${CURL_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME}
    PUBLIC ${CURL_LIBRARIES})
endif()


install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
