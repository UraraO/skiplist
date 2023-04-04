add_executable(skiplist main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
# add pthread lib
find_package(Threads REQUIRED)
target_link_libraries(skiplist PUBLIC Threads::Threads)