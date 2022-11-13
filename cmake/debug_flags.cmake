# Set compiler flags for Debug builds.
# Only has an effect on GCC/Clang >= 5.0.

set(DEBUG_CXXFLAGS "")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"
    AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5")
  list(APPEND DEBUG_CXXFLAGS
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-Wuninitialized"
    "-Wshadow"
    "-Wnon-virtual-dtor"
    "-Wconversion"
    "-Wsign-conversion"
    "-Wold-style-cast"
    "-Wzero-as-null-pointer-constant"
    "-Wmissing-declarations"
    "-Wcast-align"
    "-Wunused"
    "-Woverloaded-virtual"
    "-Wdouble-promotion"
    "-Wformat=2"
    "-ftrapv"
    "-Og"
    "-fno-omit-frame-pointer")
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    list(APPEND DEBUG_CXXFLAGS
      "-Wlogical-op"
      "-Wuseless-cast")
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6")
      list(APPEND DEBUG_CXXFLAGS
        "-Wmisleading-indentation"
        "-Wduplicated-cond"
        "-Wnull-dereference")
      if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7")
        list(APPEND DEBUG_CXXFLAGS
          "-Wduplicated-branches")
      endif()
    endif()
  endif()
  if(NOT MINGW)
    list(APPEND DEBUG_CXXFLAGS
      "-fsanitize=undefined")
  endif()
  add_compile_options("$<$<CONFIG:Debug>:${DEBUG_CXXFLAGS}>")

  if(NOT MINGW)
    set(DEBUG_LDFLAGS
      "-fsanitize=undefined")
    # add_link_options was introduced in version 3.13.
    if(${CMAKE_VERSION} VERSION_LESS 3.13)
      set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${DEBUG_LDFLAGS}")
    else()
      add_link_options("$<$<CONFIG:Debug>:${DEBUG_LDFLAGS}>")
    endif()
  endif()
else()
  message(STATUS
    "No additional compiler flags were set, "
    "because your compiler was not anticipated.")
endif()
