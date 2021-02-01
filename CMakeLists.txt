cmake_minimum_required(VERSION 3.10)
project(fireflow)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

 
# Locate GTest
find_package(GTest REQUIRED)

#Link fireflow_test with what we want to test
file(GLOB SRC_TEST "src/*.cpp" "test/*.cpp")
list(FILTER SRC_TEST EXCLUDE REGEX ".*fireflow.cpp$")
add_executable(fireflow_test ${SRC_TEST})
target_include_directories(fireflow_test PRIVATE ${GTEST_INCLUDE_DIRS} "include")
target_link_libraries(fireflow_test ${GTEST_LIBRARIES} pthread pfring pcap)