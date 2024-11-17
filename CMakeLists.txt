cmake_minimum_required(VERSION 3.10)
project(SDLEngine)

set(CMAKE_CXX_STANDARD 14)

# Include SDL2 headers and libraries
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Manually include SDL2_image paths
include_directories(/usr/include/SDL2)
set(SDL2_IMAGE_LIB /usr/lib/x86_64-linux-gnu/libSDL2_image.so)

# Manually include SDL2_ttf paths
include_directories(/usr/include/SDL2)
set(SDL2_TTF_LIB /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so)

# Add the executable and link libraries
add_executable(flappy main.cpp)
target_link_libraries(flappy ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIB} ${SDL2_TTF_LIB})
