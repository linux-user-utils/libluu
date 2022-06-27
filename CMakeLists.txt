cmake_minimum_required(VERSION 3.23)
project(libluu
  VERSION 0.0.1
  DESCRIPTION "Some useful functions"
  LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BIG_WARN "Use strict compiler flags" ON)

add_library (luu SHARED
  src/backlight.cpp
)
set_target_properties(luu
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION}
)
if (BIG_WARN)
  target_compile_options( luu
    PRIVATE -Wall -Wextra -pedantic -Werror
  )
endif (BIG_WARN)
include_directories (./include/)
include(GNUInstallDirs)
install(TARGETS luu)