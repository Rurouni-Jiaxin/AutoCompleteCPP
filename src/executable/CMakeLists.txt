add_executable(autocomplete src/main.cpp)

list(APPEND jf_SOURCES
  PRIVATE jf_lib
)

target_link_libraries(autocomplete ${jf_SOURCES} ${LIBS})
