enable_testing()
find_package(GTest REQUIRED)

add_executable(tests tests.cpp findTest.cpp ImageProcessorTest.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src)

find_package(Threads REQUIRED)
find_package(X11 REQUIRED)
target_link_libraries(tests PRIVATE gtest gtest_main gmock gmock_main Threads::Threads X11 src)
