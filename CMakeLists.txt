cmake_minimum_required(VERSION 3.10)
project(fireflow)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

 
# Locate GTest
find_package(GTest REQUIRED)

#Link fireflow_test with what we want to test

file(GLOB SRC "src/*.cpp")
list(FILTER SRC EXCLUDE REGEX ".*fireflow_test.cpp$")
add_executable(fireflow ${SRC})
target_include_directories(fireflow PRIVATE "include")
target_link_libraries(fireflow pthread pfring pcap)