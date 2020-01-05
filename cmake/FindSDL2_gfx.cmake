# FindSDL2_gfx.cmake
# ------------------
#
# Find SDL_gfx library, this modules defines:
#
# SDL_GFX_INCLUDE_DIRS, where to find SDL_gfx.h
# SDL_GFX_LIBRARIES, where to find library
# SDL_GFX_FOUND, if it is found

find_path(
        SDL2_GFX_INCLUDE_DIR
        NAMES SDL2_gfxPrimitives.h SDL2_gfxBlitFunc.h SDL2_gfxPrimitives_font.h
        PATH_SUFFIXES SDL2
)

find_library(
        SDL2_GFX_LIBRARY
        NAMES libSDL2_gfx SDL2_gfx
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
        SDL2_gfx
        FOUND_VAR SDL2_GFX_FOUND
        REQUIRED_VARS SDL2_GFX_LIBRARY SDL2_GFX_INCLUDE_DIR
)

set(SDL2_GFX_LIBRARIES ${SDL2_GFX_LIBRARY})
set(SDL2_GFX_INCLUDE_DIRS ${SDL2_GFX_INCLUDE_DIR})

mark_as_advanced(SDL2_GFX_INCLUDE_DIR SDL2_GFX_LIBRARY)