### Explorer Extension #######################################################

add_executable(test_ext
  src/test_ext.cpp
)

format_output_name(test_ext "test_ext")

set_target_properties(test_ext PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

target_link_libraries(test_ext
  PRIVATE csUtil
)

### Win32 ####################################################################

add_executable(test_win32 WIN32
  src/test_win32.cpp
  ${csMenu3_SOURCE_DIR}/Win32Compat/resource/Dialog.rc
  src/RenameDialog.h
  src/RenameDialog.cpp
  src/RenameDialogResource.h
  src/RenameDialog.rc
)

format_output_name(test_win32 "test_win32")

set_target_properties(test_win32 PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

target_link_libraries(test_win32
  PRIVATE Win32Compat
)
