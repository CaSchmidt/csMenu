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
