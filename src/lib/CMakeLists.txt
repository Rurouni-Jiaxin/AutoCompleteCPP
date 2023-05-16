add_library(jf_lib STATIC
  src/Dictionary.cpp
  src/Trie.cpp
  )


target_include_directories(jf_lib PUBLIC STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src")