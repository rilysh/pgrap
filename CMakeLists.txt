cmake_minimum_required(VERSION 3.12)
project(pgrap)

# Set the C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (USE_CLANG)
  set(CMAKE_CXX_COMPILER clang++)
endif()

if (DEBUG_BUILD)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Og -ggdb3")
else()
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -s -O3")
endif()

add_executable(pgrap
  src/pgrap.cpp
  src/simple_match.cpp
  src/stand_match.cpp
  src/hs_match.cpp
)

target_include_directories(pgrap PUBLIC deps/fmt/include/ deps/vectorscan/src/)

target_link_directories(pgrap PUBLIC ./deps/fmt/ ./deps/vectorscan/lib/)
target_link_libraries(pgrap PUBLIC fmt hs)
