cmake_minimum_required(VERSION 3.10.2)
project(autocomplete_jf VERSION 1.0.0)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

message("-----------------------------")
message(${CMAKE_CXX_COMPILER_ID})
message("-----------------------------")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-override")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wno-error=unused-variable -Wno-error=unused-but-set-variable -Werror=suggest-override")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Werror=header-hygiene -Wno-c++98-compat -Wno-shadow -Wno-c++98-compat-pedantic -Wno-date-time -Wno-missing-prototypes -Wno-exit-time-destructors -Wno-global-constructors -Wno-conversion -Wno-sign-conversion -Wno-float-equal -Wno-padded -Wno-weak-vtables -Wno-abstract-vbase-init -Wno-potentially-evaluated-expression -Wno-documentation-unknown-command")
endif()



if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

message("############### BUILD TYPE: ${CMAKE_BUILD_TYPE} ###############")

cmake_minimum_required(VERSION 2.8.11)


# Find includes in corresponding build directories
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(src/lib)

#executable
add_subdirectory(src/executable)
