## Get all variables visible to CMake and print them
## Source: https://stackoverflow.com/a/9328525/3214870
## get_cmake_property(_variableNames VARIABLES)
## list(SORT _variableNames)
## foreach (_variableName ${_variableNames})
##     message(STATUS "${_variableName}=${${_variableName}}")
## endforeach ()

cmake_minimum_required(VERSION 3.15)
project(tetris)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${tetris_SOURCE_DIR}/cmake)

# SDL2
FIND_PACKAGE(SDL2 REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

# SDL2 Gfx
FIND_PACKAGE(SDL2_gfx REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_GFX_INCLUDE_DIRS})


include_directories(.)

add_executable(tetris main.c pieces.inl primlib.c primlib.h game.c game.h userInterface.c userInterface.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARIES})
