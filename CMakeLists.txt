cmake_minimum_required(VERSION 3.10)
project(IOD)

set(CMAKE_CXX_STANDARD 17)


if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.
  Please create a build directory and run cmake from there:
  
      mkdir build && cd build && cmake ..")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src/commands)

set(SRC_FILES
    src/main.cpp
    src/commands/IndexCommand.cpp
    src/commands/SearchCommand.cpp
    src/Document.cpp
    src/Tokeniser.cpp
    src/InvertedIndex.cpp
)

add_executable(iod ${SRC_FILES})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    target_link_libraries(my_executable stdc++fs)
  endif()
endif()
