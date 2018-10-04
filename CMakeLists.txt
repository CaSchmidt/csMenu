cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(csMenu-dev)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(csMenu2)

set_target_properties(csMenu PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
  )
