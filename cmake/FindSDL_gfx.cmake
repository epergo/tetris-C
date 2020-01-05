# FindSDL_gfx.cmake
# ------------------
#
# Find SDL_gfx library, this modules defines:
#
# SDL_GFX_INCLUDE_DIRS, where to find SDL_gfx.h
# SDL_GFX_LIBRARIES, where to find library
# SDL_GFX_FOUND, if it is found

find_path(
        SDL_GFX_INCLUDE_DIR
        NAMES SDL_gfxPrimitives.h SDL_gfxBlitFunc.h SDL_gfxPrimitives_font.h
        PATH_SUFFIXES SDL
)

find_library(
        SDL_GFX_LIBRARY
        NAMES libSDL_gfx SDL_gfx
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
        SDL_gfx
        FOUND_VAR SDL_GFX_FOUND
        REQUIRED_VARS SDL_GFX_LIBRARY SDL_GFX_INCLUDE_DIR
)

set(SDL_GFX_LIBRARIES ${SDL_GFX_LIBRARY})
set(SDL_GFX_INCLUDE_DIRS ${SDL_GFX_INCLUDE_DIR})

mark_as_advanced(SDL_GFX_INCLUDE_DIR SDL_GFX_LIBRARY)