cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

### Project ##################################################################

project(csMenu)

# Custom CMake utilities
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FormatOutputName)

# C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

### csMenu ###################################################################

list(APPEND csMenu_HEADERS
  include/clipboard.h
  include/command.h
  include/csCore2/csFileUtil.h
  include/csCore2/csProcess.h
  include/csCore2/csWString.h
  include/csMenu.h
  include/csMenuFactory.h
  include/menu.h
  include/reg.h
  include/util.hpp
  include/worker/IJobQueue.h
  include/worker/ParallelExecutor.h
  include/worker/ProgressUI.h
  include/worker/worker.h
  )

list(APPEND csMenu_SOURCES
  src/clipboard.cpp
  src/command.cpp
  src/csCore2/csFileUtil_win32.cpp
  src/csCore2/csProcess_win32.cpp
  src/csMenu.cpp
  src/csMenuFactory.cpp
  src/main.cpp
  src/menu.cpp
  src/reg.cpp
  src/util.cpp
  src/worker/IJobQueue.cpp
  src/worker/ParallelExecutor.cpp
  src/worker/ProgressUI.cpp
  src/worker/batch_work.cpp
  src/worker/parallel_work.cpp
  src/worker/sequential_work.cpp
  src/worker/worker.cpp
  )

list(APPEND csMenu_EXTRAS
  csMenu.def
  resource/csMenu.rc
  resource/resource.h
  )

### Target ###################################################################

add_library(csMenu SHARED
  ${csMenu_HEADERS} ${csMenu_SOURCES}
  ${csMenu_EXTRAS}
  )

format_output_name(csMenu "csMenu")

target_link_libraries(csMenu comctl32.lib mpr.lib shlwapi.lib)

target_compile_definitions(csMenu PRIVATE _CRT_SECURE_NO_WARNINGS)
target_include_directories(csMenu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(csMenu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/resource)
