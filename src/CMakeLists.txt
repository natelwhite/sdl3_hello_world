# output location of executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# all .cpp files
set(
  sources
  main.cpp
  Renderer.cpp
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${sources})
