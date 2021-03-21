cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

project(bill-bill-bill)

add_executable(bill-bill-bill main.cpp)
add_subdirectory(src)
add_subdirectory(tests)

# Deps
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)
find_package(cxxopts REQUIRED)
target_link_libraries(bill-bill-bill PRIVATE Threads::Threads X11 src src_cli)
