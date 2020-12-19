set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)

cmake_minimum_required(VERSION 3.13)
project(mushroom)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(hashes)

include_directories(hashes/include)
link_directories(hashes/src)


add_subdirectory(tools/googletest)
add_subdirectory(tests)

add_executable(mushroom_run main.cpp)
target_link_libraries(mushroom_run mushroom_lib)