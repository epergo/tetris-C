#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

enum color { BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, COLOR_MAX };

int initGraph(char *title);

void freeResources();

void clearScreen();

void updateScreen();

void filledRect(int x1, int y1, int x2, int y2, enum color c);
void filledRectRGB(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);

void rect(int x1, int y1, int x2, int y2, enum color c);

void rectRGB(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);

void textOut(int x, int y, char *s, enum color c);

int screenWidth();

int screenHeight();

int getKey();
