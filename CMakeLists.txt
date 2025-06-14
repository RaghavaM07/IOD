cmake_minimum_required(VERSION 3.10)
project(IOD)

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
)

add_executable(iod ${SRC_FILES})
