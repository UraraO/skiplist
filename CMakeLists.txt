cmake_minimum_required(VERSION 3.24)
project(skiplist)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src ../bin)
add_subdirectory(stress_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
# add pthread lib
find_package(Threads REQUIRED)
target_link_libraries(skiplist PUBLIC Threads::Threads)
