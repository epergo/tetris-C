#ifndef TETRIS__USER_INTERFACE_H
#define TETRIS__USER_INTERFACE_H

#include "game.h"
#include <stdbool.h>

#define SQUARE_WIDTH 20

void drawField(int[F_COLS][F_ROWS]);
void drawControlsAndScore(int);
void drawNextPiece(const char[7][4][4][4], int);

#endif // TETRIS__USER_INTERFACE_H
