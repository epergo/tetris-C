cmake_minimum_required(VERSION 3.15)
project(tetris C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${tetris_SOURCE_DIR}/cmake)

# SDL2
FIND_PACKAGE(SDL2 REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR})

# SDL2 Gfx
FIND_PACKAGE(SDL2_gfx REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_GFX_INCLUDE_DIRS})

include_directories(.)

add_executable(tetris
        main.c
        pieces.inl
        primlib.c
        primlib.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_GFX_LIBRARIES})
