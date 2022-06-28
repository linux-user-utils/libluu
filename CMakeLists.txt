cmake_minimum_required(VERSION 3.23)
project(libluu
  VERSION 0.0.1
  DESCRIPTION "Some useful functions"
  LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BIG_WARN "Use strict compiler flags" ON)
include_directories(./include/) # For #include <foo> in IDE


## Build Library ##




include_directories(include)

add_library (luu SHARED
  src/backlight.cpp
  src/utility.cpp
)

configure_file(src/version.hpp.in version.hpp)
target_include_directories(luu PRIVATE ${CMAKE_CURRENT_BINARY_DIR}) # For version.hpp

set_target_properties(luu
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION}
  PUBLIC_HEADER 
"include/luu/backlight.hpp;include/luu/concepts.hpp;include/luu/logging.hpp;include/luu/parse_arguments.hpp;include/luu/utility.hpp"
)

# # Set compiler warnings ##
if(  CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
  OR CMAKE_BUILD_TYPE STREQUAL "Debug")
if (BIG_WARN)
  target_compile_options( luu
    PRIVATE -Wall -Wextra -pedantic -Werror
  )
endif (BIG_WARN)
endif ()

## Installation ##
configure_file(src/libluu.pc.in libluu.pc @ONLY)
include(GNUInstallDirs)
install(TARGETS luu
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libluu/
)

## pkgconfig ##
install(FILES ${CMAKE_BINARY_DIR}/libluu.pc
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig/)