set(SDL_TEST_LIBRARY OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(SDL)

set(SDLSHADERCROSS_SPIRVCROSS_SHARED OFF)
set(SDLSHADERCROSS_VENDORED ON)
add_subdirectory(SDL_shadercross)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE SDL3::SDL3-static SDL3_shadercross-static)
