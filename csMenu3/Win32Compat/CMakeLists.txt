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
  include/Win32/UI/CheckBox.h
  include/Win32/UI/ComboBox.h
  include/Win32/UI/Dialog.h
  include/Win32/UI/EditText.h
  include/Win32/UI/Window.h
  include/Win32/WindowUtil.h
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
  src/UI/CheckBox.cpp
  src/UI/ComboBox.cpp
  src/UI/Dialog.cpp
  src/UI/EditText.cpp
  src/UI/Window.cpp
  src/WindowUtil.cpp
)

list(APPEND Win32Compat_OTHER
  resource/DialogResource.h
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
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/resource
)

target_link_libraries(Win32Compat
  PRIVATE comctl32.lib
  PRIVATE mpr.lib
)

target_sources(Win32Compat
  PRIVATE ${Win32Compat_HEADERS}
  PRIVATE ${Win32Compat_SOURCES}
  PRIVATE ${Win32Compat_OTHER}
)
