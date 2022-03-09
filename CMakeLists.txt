cmake_minimum_required(VERSION 3.14)
project(circular_list)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

include(CTest)
include(GoogleTest)

add_executable(
  circular_list_test
  circular_list_test.cpp
)
target_link_libraries(
  circular_list_test
  gtest_main
)
gtest_discover_tests(circular_list_test)
