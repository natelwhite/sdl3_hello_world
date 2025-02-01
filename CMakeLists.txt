cmake_minimum_required(VERSION 3.24)
project(sdl3_hello_world)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${CMAKE_PROJECT_NAME})

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(include)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
