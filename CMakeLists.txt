cmake_minimum_required(VERSION 3.10)
set(TARGET syaz0)
project(${TARGET} CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(${TARGET} STATIC
  src/include/types.h
  src/include/yaz0.h
  src/yaz0.cpp
)

target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

if(MSVC)
  target_compile_options(${TARGET} PRIVATE /W4 /wd4244 /wd4127 /Zc:__cplusplus)
else()
  target_compile_options(${TARGET} PRIVATE -Wall -Wextra -fno-plt)
endif()

option(ZLIB_ENABLE_TESTS "" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/zlib-ng)
target_link_libraries(${TARGET} PRIVATE zlib)
