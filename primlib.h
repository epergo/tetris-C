#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

int initGraph(char *title);

void freeResources();

void clearScreen();

void updateScreen();

void rect(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);

void filledRect(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);

void textOut(int x, int y, char *s, Uint8 r, Uint8 g, Uint8 b);

int screenWidth();

int screenHeight();

int getKey();
