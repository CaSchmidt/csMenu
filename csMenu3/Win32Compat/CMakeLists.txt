### Project ##################################################################

list(APPEND Win32Compat_HEADERS
  include/Win32/Clipboard.h
  include/Win32/Compat.h
  include/Win32/GUID.h
  include/Win32/Message.h
  include/Win32/MessageBox.h
  include/Win32/Module.h
  include/Win32/Network.h
  include/Win32/ProgressBar.h
  include/Win32/Registry.h
  include/Win32/Shell.h
  include/Win32/String.h
)

list(APPEND Win32Compat_SOURCES
  src/Clipboard.cpp
  src/Compat.cpp
  src/GUID.cpp
  src/Message.cpp
  src/MessageBox.cpp
  src/Module.cpp
  src/Network.cpp
  src/ProgressBar.cpp
  src/Registry.cpp
  src/Shell.cpp
  src/String.cpp
)

### Target ###################################################################

add_library(Win32Compat STATIC)

format_output_name(Win32Compat "Win32Compat")

set_target_properties(Win32Compat PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

target_include_directories(Win32Compat
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#target_link_libraries(csMenu3
#  PRIVATE csUtil
#  PRIVATE comctl32.lib
#  PRIVATE mpr.lib
#  PRIVATE shlwapi.lib
#)

target_sources(Win32Compat
  PRIVATE ${Win32Compat_HEADERS}
  PRIVATE ${Win32Compat_SOURCES}
)
