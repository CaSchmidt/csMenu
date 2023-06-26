### Project ##################################################################

list(APPEND DebugSupport_HEADERS
  include/Debug/Debug.h
)

list(APPEND DebugSupport_SOURCES
  src/Debug.cpp
)

### Target ###################################################################

add_library(DebugSupport STATIC)

format_output_name(DebugSupport "DebugSupport")

set_target_properties(DebugSupport PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

target_include_directories(DebugSupport
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#target_link_libraries(DebugSupport
#  PRIVATE comctl32.lib
#  PRIVATE mpr.lib
#)

target_sources(DebugSupport
  PRIVATE ${DebugSupport_HEADERS}
  PRIVATE ${DebugSupport_SOURCES}
)
