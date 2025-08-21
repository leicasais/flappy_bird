cmake_minimum_required(VERSION 3.10)
project(FlappyBirdClone C)

# Use the C11 standard
set(CMAKE_C_STANDARD 11)

# Include directories for headers, tells the compiler where to look for header files when it sees an #include "..." directive in your C code
include_directories(
    backend
    frontend
)

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

# Source files, specify each doc and if its in a file
set(SOURCES
    main.c
    backend/bird_fun.c
    backend/column_fun.c
    backend/game_logic.c
    backend/init_fun.c
    backend/menu.c
    frontend/frontend.c
)

# Build the executable
add_executable(flappy_bird ${SOURCES})

# Link SDL2
target_link_libraries(flappy_bird ${SDL2_LIBRARIES} SDL2_image)
target_link_libraries(flappy_bird ${SDL2_LIBRARIES} SDL2_image SDL2_ttf)
