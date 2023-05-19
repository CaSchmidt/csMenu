cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(csMenu3)

### CMake ####################################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FormatOutputName)

### Dependencies #############################################################

add_subdirectory(${csMenu-dev_SOURCE_DIR}/3rdparty/csUtil/csUtil
  ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/csUtil
)

### Project ##################################################################

list(APPEND csMenu3_EXTRAS
  csMenu3.def
)

list(APPEND csMenu3_HEADERS
  include/CommandBase.h
  include/CommandEnum.h
  include/CommandFlag.h
  include/CommandInvoke.h
  include/Commands.h
  include/CommandSeparator.h
  include/guid.h
  include/Win32/Compat.h
  include/Win32/Registry.h
)

list(APPEND csMenu3_SOURCES
  src/CommandBase.cpp
  src/CommandEnum.cpp
  src/CommandFlag.cpp
  src/CommandInvoke.cpp
  src/Commands.cpp
  src/CommandSeparator.cpp
  src/guid.cpp
  src/main.cpp
  src/Win32/Compat.cpp
  src/Win32/Registry.cpp
)

### Target ###################################################################

add_library(csMenu3 SHARED)

format_output_name(csMenu3 "csMenu")

set_target_properties(csMenu3 PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

target_include_directories(csMenu3
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(csMenu3
  PRIVATE csUtil shlwapi.lib
)

target_sources(csMenu3
  PRIVATE ${csMenu3_EXTRAS}
  PRIVATE ${csMenu3_HEADERS}
  PRIVATE ${csMenu3_SOURCES}
)
