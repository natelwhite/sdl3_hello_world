set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
add_executable(${CMAKE_PROJECT_NAME})
target_sources(${CMAKE_PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
