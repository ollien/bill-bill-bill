cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(bill-bill-bill)

add_executable(bill-bill-bill main.cpp)
add_subdirectory(src)
add_subdirectory(tests)

# Deps
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)
find_package(cxxopts REQUIRED)
target_link_libraries(bill-bill-bill PRIVATE Threads::Threads X11 src)
