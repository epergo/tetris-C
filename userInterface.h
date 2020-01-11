#ifndef TETRIS__USER_INTERFACE_H
#define TETRIS__USER_INTERFACE_H

#include "game.h"
#include <stdbool.h>

#define SQUARE_WIDTH 20

void drawField(int field[F_COLS][F_ROWS]);
void drawControls();
void drawNextPiece(int nextPiece);
void drawEndGame(int score);
void drawScore(int score);

#endif // TETRIS__USER_INTERFACE_H
