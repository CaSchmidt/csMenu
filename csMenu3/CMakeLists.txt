cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(csMenu3)

### CMake ####################################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FormatOutputName)

### Dependencies #############################################################

add_subdirectory(${csMenu-dev_SOURCE_DIR}/3rdparty/csUtil/csUtil
  ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/csUtil
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/DebugSupport
  ${CMAKE_CURRENT_BINARY_DIR}/DebugSupport
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Win32Compat
  ${CMAKE_CURRENT_BINARY_DIR}/Win32Compat
)

### Project ##################################################################

list(APPEND csMenu3_EXTRAS
  csMenu3.def
  resource/csMenu3.rc
  resource/csMenu3Resource.h
  resource/RenameDialog.rc
  resource/RenameDialogResource.h
)

list(APPEND csMenu3_HEADERS
  include/CommandBase.h
  include/CommandEnum.h
  include/CommandFlag.h
  include/CommandInvoke.h
  include/Commands.h
  include/CommandSeparator.h
  include/FileName.h
  include/GUIDs.h
  include/HashMenuFactory.h
  include/HashWorker.h
  include/Invoke.h
  include/MainMenuFactory.h
  include/MenuFlags.h
  include/Register.h
  include/Rename.h
  include/RenameDialog.h
  include/ScriptMenuFactory.h
  include/ScriptWorker.h
  include/Settings.h
  include/Util.h
  include/WorkContext.h
)

list(APPEND csMenu3_SOURCES
  src/CommandBase.cpp
  src/CommandEnum.cpp
  src/CommandFlag.cpp
  src/CommandInvoke.cpp
  src/Commands.cpp
  src/CommandSeparator.cpp
  src/FileName.cpp
  src/GUIDs.cpp
  src/HashMenuFactory.cpp
  src/HashWorker.cpp
  src/Invoke.cpp
  src/main.cpp
  src/MainMenuFactory.cpp
  src/MenuFlags.cpp
  src/Register.cpp
  src/Rename.cpp
  src/RenameDialog.cpp
  src/ScriptMenuFactory.cpp
  src/ScriptWorker.cpp
  src/WorkContext.cpp
)

### Target ###################################################################

add_library(csMenu3 SHARED)

format_output_name(csMenu3 "csMenu3")

set_target_properties(csMenu3 PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

target_include_directories(csMenu3
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/resource
)

target_link_libraries(csMenu3
  PRIVATE csUtil
  PRIVATE Win32Compat
  PRIVATE shlwapi.lib
)

target_sources(csMenu3
  PRIVATE ${csMenu3_EXTRAS}
  PRIVATE ${csMenu3_HEADERS}
  PRIVATE ${csMenu3_SOURCES}
)

### Tests ####################################################################

add_subdirectory(tests)
