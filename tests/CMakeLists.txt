enable_testing()
find_package(GTest REQUIRED)

add_executable(tests tests.cpp findTest.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src)

message(${GTEST_LIBRARIES})
message(${GTEST_BOTH_LIBRARIES})

target_link_libraries(tests gtest gtest_main gmock gmock_main src)
