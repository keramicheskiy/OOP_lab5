cmake_minimum_required(VERSION 3.10)
project(Alloc)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)


include_directories(include source)

set(MAIN_SOURCE main.cpp)
set(TESTS_SOURCE tests.cpp)

add_executable(main ${MAIN_SOURCE})# ${LIB_SOURCES})
# target_link_libraries(main PUBLIC fmt::fmt)# figures)


add_executable(tests ${TESTS_SOURCE})
target_link_libraries(tests PUBLIC gtest_main)



gtest_discover_tests(tests)