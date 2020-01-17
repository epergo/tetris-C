#include <stdio.h>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "audio/audio.h"
#include "primlib.h"

static SDL_Window *sdlWindow;
static SDL_Renderer *sdlRenderer;

void rect(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
  rectangleRGBA(sdlRenderer, x1, y1, x2 + 1, y2 + 1, r, g, b, SDL_ALPHA_OPAQUE);
}

void filledRect(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
  boxRGBA(sdlRenderer, x1, y1, x2, y2, r, g, b, SDL_ALPHA_OPAQUE);
}

void clearScreen() {
  SDL_SetRenderDrawColor(sdlRenderer, 10, 10, 10, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(sdlRenderer);
}

void updateScreen() { SDL_RenderPresent(sdlRenderer); }

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

void textOut(int x, int y, char *s, Uint8 r, Uint8 g, Uint8 b) {
  stringRGBA(sdlRenderer, x, y, s, r, g, b, 255);
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

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  // Create window
  sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,
                               SDL_WINDOW_SHOWN);

  if (sdlWindow == NULL) {
    fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
  if (sdlRenderer == NULL) {
    fprintf(stderr, "Error when obtaining the renderer, SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  initAudio();

  return 0;
}

void freeResources() {
  SDL_DestroyWindow(sdlWindow);
  SDL_Quit();
}
