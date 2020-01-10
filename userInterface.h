#include "game.h"
#include <stdbool.h>

#define SQUARE_WIDTH 20

void drawField(int[F_COLS][F_ROWS], bool);
void drawControlsAndScore(int);
void drawNextPiece(char[7][4][4][4], int);
