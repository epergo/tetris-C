#include <stdio.h>
#include <assert.h>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "primlib.h"

static SDL_Window *sdlWindow;
static SDL_Renderer *sdlRenderer;

static Uint32 colors[COLOR_MAX] = {
        0xff0000ff,
        0xff0000ff,
        0xceceb7ff,
        0xfdf498ff,
        0x7bc043ff,
        0xff00ffff,
        0xffff00ff,
        0xffffffff
};

void rect(int x1, int y1, int x2, int y2, enum color c) {
    assert(c < COLOR_MAX);
    rectangleColor(sdlRenderer, x1, y1, x2, y2, colors[c]);
}

void rectRGB(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    rectangleRGBA(sdlRenderer, x1, y1, x2, y2, r, g, b, 255);
}

void filledRect(int x1, int y1, int x2, int y2, enum color c) {
    assert(c < COLOR_MAX);
    boxColor(sdlRenderer, x1, y1, x2, y2, colors[c]);
}

void filledRectRGB(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    boxRGBA(sdlRenderer, x1, y1, x2, y2, r, g, b, 255);
}

void clearScreen() {
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdlRenderer);
}

void updateScreen() {
    SDL_RenderPresent(sdlRenderer);
}

int screenWidth() {
    int w;
    int h;
    SDL_GetWindowSize(sdlWindow, &w, &h);
    return w;
}

int screenHeight() {
    int w;
    int h;
    SDL_GetWindowSize(sdlWindow, &w, &h);
    return h;
}

void textOut(int x, int y, char *s, enum color c) {
    assert(c < COLOR_MAX);
    stringColor(sdlRenderer, x, y, s, colors[c]);
}

int getKey() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return -1;
        }
        if (e.type == SDL_KEYDOWN) {
            return e.key.keysym.sym;
        }
    }

    return 0;
}

int initGraph(char *title) {
    if (sdlWindow) {
        fprintf(stderr, "initGraph called twice\n");
        return 1;
    }

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
    } else {
        //Create window
        sdlWindow = SDL_CreateWindow(
                title,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                800,
                600,
                SDL_WINDOW_SHOWN
        );
        if (sdlWindow == NULL) {
            fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
            if (sdlRenderer == NULL) {
                fprintf(stderr, "Error when obtaining the renderer, SDL_Error: %s\n", SDL_GetError());
                return 1;
            }
        }
    }

    return 0;
}

void freeResources() {
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}
