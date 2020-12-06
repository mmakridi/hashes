cmake_minimum_required(VERSION 3.13)
project(mushroom_hash)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(hashes)

#include_directories(./include)

#set(SOURCES main.cpp include/hash_table.hpp include/hash_table_list.hpp)
#set(SOURCES main.cpp)

#add_executable(mushroom_hash ${SOURCES})


include_directories(hashes/include)
link_directories(hashes/src)
add_executable(main main.cpp)
target_link_libraries(main lib_hash)