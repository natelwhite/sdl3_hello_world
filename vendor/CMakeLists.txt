set(SDL_TEST_LIBRARY OFF)
option(BUILD_SHARED_LIBS OFF)
add_subdirectory(SDL)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE SDL3::SDL3)
