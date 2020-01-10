#include "game.h"
#include "pieces.inl"
#include "primlib.h"
#include "userInterface.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/* Global variables */
bool drawGrill = false;
bool playing = true;
int rotation = 0;
int score = 0;

int field[F_COLS][F_ROWS];
int actualC;
int actualR;
double actualY;
int actualPiece;
int nextPiece;

int play() {
  /* Init the seed for random numbers */
  srand(time(NULL));

  /* Init graphics */
  if (initGraph("It's Tetris!")) {
    exit(3);
  }

  /* Puts all the positions to 0 */
  initField();

  /* Getting the first piece */
  actualPiece = rand() % 7;
  nextPiece = rand() % 7;
  setNewPieceInField(actualPiece);

  do {
    clearScreen();
    int keyPressed = getKey();
    if (keyPressed == -1) {
      playing = false;
    } else {
      movePiece(keyPressed);
      drawField(field, drawGrill);
      drawControlsAndScore(score);
      drawNextPiece(pieces, nextPiece);

      updateScreen();
      SDL_Delay(DELAY);
    }
  } while (playing);

  freeResources();
}

/* INIT FIELD FUNCTION */
void initField() {
  for (int counter_c = 0; counter_c < F_COLS; counter_c++) {
    for (int counter_f = 0; counter_f < F_ROWS; counter_f++) {
      field[counter_c][counter_f] = 0;
    }
  }
}

/* LOGIC FUNCTIONS */
int setNewPieceInField(int type) {
  actualC = 3;
  actualR = 0;
  actualY = 0;
  rotation = 0;

  for (int counterC = 0; counterC < 4; counterC++) {
    for (int counterR = 0; counterR < 4; counterR++) {
      if (field[actualC + counterC][counterR] == 0) {
        field[actualC + counterC][counterR] = pieces[type][rotation][counterC][counterR];
      } else {
        // Tile already occupied, GameOver
        return -1;
      }
    }
  }

  return 0;
}

void movePiece(int key) {
  int wait;
  switch (key) {
  case SDLK_1:
    drawGrill = !drawGrill;
    break;
  case SDLK_LEFT:
    moveLeft();
    moveDown();
    break;
  case SDLK_RIGHT:
    moveRight();
    break;
  case SDLK_SPACE:
    do {
      wait = moveDown();
    } while (wait != 1);
    break;
  case SDLK_RETURN:
    rotatePiece();
    break;
  case SDLK_ESCAPE:
    playing = false;
    exit(0);
  default:
    moveDown();
    break;
  }
}

void moveLeft() {
  bool isPossible = true;
  // The following nested `for` decides if the movement is possible or not
  for (int counterC = 0; counterC < 4 && isPossible; counterC++) {
    for (int counterR = 0; counterR < 4; counterR++) {
      if (actualC + counterC <= 0) {
        // We have reached the limit of the board, movement not possible
        isPossible = false;
        break;
      }

      int insidePosition = field[actualC + counterC][actualR + counterR];
      if (insidePosition == 1) {
        int insidePositionLeft = field[actualC + counterC - 1][actualR + counterR];
        if (insidePositionLeft == 3) {
          // Tile already occupied by another piece, movement is not possible
          isPossible = false;
          break;
        }
      }
    }
  }

  if (!isPossible) {
    return;
  }

  // Movement is possible, apply it
  for (int counterC = 0; counterC < 4; counterC++) {
    for (int counterR = 0; counterR < 4; counterR++) {
      int insidePosition = field[actualC + counterC][actualR + counterR];
      if (actualC + counterC == F_COLS || (insidePosition != 1 && insidePosition != 2)) {
        continue;
      }

      field[actualC + counterC - 1][actualR + counterR] = insidePosition;

      // Update what was in my position to be empty unless another part of the
      // current piece is there
      if (actualC + counterC + 1 < F_COLS) {
        int insidePositionRight = field[actualC + counterC + 1][actualR + counterR];
        if (insidePositionRight == 0 || insidePositionRight == 3) {
          field[actualC + counterC][actualR + counterR] = 0;
        }
      } else {
        field[actualC + counterC][actualR + counterR] = 0;
      }
    }
  }

  actualC -= 1;
}

void moveRight() {
  int isPossible = 1; /* 1 is possible, 0 no */

  for (int counterC = 0; counterC < 4 && isPossible == 1; counterC++) {
    for (int counterR = 0; counterR < 4 && isPossible == 1; counterR++) {
      if (actualC + counterC < F_COLS) {
        int insidePosition;
        int insidePositionRight;

        insidePosition = field[actualC + counterC][actualR + counterR];

        if (insidePosition == 1) {
          if (actualC + counterC + 1 < F_COLS) {
            insidePositionRight = field[actualC + counterC + 1][actualR + counterR];

            if (insidePositionRight != 3) {
              /* OK */
            } else {
              isPossible = 0;
              break;
            }
          } else {
            isPossible = 0;
            break;
          }
        }
      }
    }
  }

  if (isPossible == 1) {
    for (int counterC = 3; counterC >= 0; counterC--) {
      for (int counterR = 0; counterR < 4; counterR++) {
        if (actualC + counterC + 1 < F_COLS) {
          int insidePosition;
          int insidePositionLeft;

          insidePosition = field[actualC + counterC][actualR + counterR];

          if (insidePosition == 1) {
            if (actualC + counterC + 1 < F_COLS) {
              field[actualC + counterC + 1][actualR + counterR] = insidePosition;

              if (actualC + counterC > 0) {
                insidePositionLeft = field[actualC + counterC - 1][actualR + counterR];

                if (insidePositionLeft == 0 || insidePositionLeft == 3) {
                  field[actualC + counterC][actualR + counterR] = 0;
                }
              } else {
                field[actualC + counterC][actualR + counterR] = 0;
              }
            }
          }
        }
      }
    }

    actualC += 1;
  }

  moveDown();
}

int moveDown() {
  int isPossible = 1;
  for (int counterC = 0; counterC < 4 && isPossible == 1; counterC++) {
    for (int counterR = 0; counterR < 4; counterR++) {
      if (actualR + counterR < F_ROWS && actualC + counterC < F_COLS) {
        if (field[actualC + counterC][actualR + counterR] == 1) {
          if (actualR + counterR > F_ROWS - 2) {
            isPossible = 0;
            break;
          } else if (field[actualC + counterC][actualR + counterR + 1] == 3) {
            isPossible = 0;
            break;
          }
        }
      }
    }
  }

  actualY += VELOCITY;

  if (actualY >= actualR * 20) {
    actualY = actualR * 20;

    if (isPossible == 1) {
      for (int counterC = 0; counterC < 4; counterC++) {
        for (int counterR = 3; counterR >= 0; counterR--) {
          if (actualR + counterR + 1 < F_ROWS && actualC + counterC < F_COLS) {
            if (field[actualC + counterC][actualR + counterR] != 3 &&
                field[actualC + counterC][actualR + counterR] != 0) {
              field[actualC + counterC][actualR + counterR + 1] =
                  field[actualC + counterC][actualR + counterR];

              if (actualR + counterR - 1 < 0 ||
                  field[actualC + counterC][actualR + counterR - 1] == 3)
                field[actualC + counterC][actualR + counterR] = 0;
              else
                field[actualC + counterC][actualR + counterR] =
                    field[actualC + counterC][actualR + counterR - 1];
            }
          }
        }
      }
      actualR += 1;
    } else {
      for (int counterC = 0; counterC < 4; counterC++) {
        for (int counterR = 0; counterR < 4 && actualR + counterR < F_ROWS; counterR++) {
          if (actualR + counterR < F_ROWS && actualC + counterC < F_COLS) {
            if (field[actualC + counterC][actualR + counterR] == 1) {
              field[actualC + counterC][actualR + counterR] = 3;
            }
          }
        }
      }

      actualPiece = nextPiece;
      nextPiece = rand() % 7;

      checkIfLine();

      if (setNewPieceInField(actualPiece) == -1) {
        endGame();
      }

      return 1;
    }
  }
  return 0;
}

void rotatePiece() {
  int pos;
  int pos_f;
  int yActual = 99;
  int exit = 0;
  int isPossible = 0;

  if (rotation == 3)
    rotation = 0;
  else
    rotation += 1;

  for (int c = 0; c < 4 && isPossible == 0; c++) {
    for (int r = 0; r < 4; r++) {
      if (actualC + c >= 0 && actualC + c < F_COLS && actualR + r >= 0 &&
          actualR + r < F_ROWS - 1) {
        pos_f = field[actualC + c][actualR + r];
        pos = pieces[actualPiece][rotation][c][r];

        if (pos_f == 3 && pos != 0) {
          isPossible = 1;
          break;
        }
      } else {
        if (pos != 0) {
          isPossible = 1;
          break;
        }
      }
    }
  }

  if (actualC + yActual < 0 || isPossible == 1) {
    /* Impossible to do the rotation */
    rotation = rotation - 1;
  } else {
    for (int c = 3; c >= 0 && exit != 1; c--) {
      for (int r = 3; r >= 0 && exit != 1; r--) {
        pos = pieces[actualPiece][rotation][c][r];

        if (actualC + c >= 0 && actualC + c < F_COLS) {
          if (actualR + r >= 0 && actualR + r < F_ROWS - 1) {
            pos_f = field[actualC + c][actualR + r];

            if (pos_f != 3) {
              field[actualC + c][actualR + r] = pos;
            } else {
              if (pos != 0)
                exit = 1;
            }
          } else {
            if (pos != 0)
              exit = 1;
          }
        } else {
          if (pos != 0)
            exit = 1;
        }
      }
    }

    if (exit == 1)
      rotation = rotation - 1;
  }
}

void checkIfLine() {
  for (int rows = F_ROWS - 1; rows >= 0; rows--) {
    bool line = true;
    for (int columns = 0; columns < F_COLS; columns++) {
      if (field[columns][rows] != 3) {
        line = false;
        break;
      }
    }

    if (!line) {
      return;
    }

    score += 50;
    for (int rows2 = rows; rows2 > 0; rows2--) {
      for (int columns = 0; columns < F_COLS; columns++) {
        if (rows2 - 1 == 0) {
          field[columns][rows2 - 1] = 0;
        }

        field[columns][rows2] = field[columns][rows2 - 1];
      }
    }
    rows++;
  }
}

void endGame() {
  initField();

  char str[100];
  sprintf(str, "%d", score);
  textOut(screenWidth() / 3 + 145, screenHeight() / 2 - 25, "SCORE", YELLOW);
  textOut(screenWidth() / 3 + 215, screenHeight() / 2 - 25, str, YELLOW);

  textOut(screenWidth() / 3 + 25, screenHeight() / 2 - 25, "GAME OVER", RED);
  textOut(screenWidth() / 3 - 25, screenHeight() / 2, "PRESS ENTER TO PLAY AGAIN OR ESC TO EXIT",
          RED);
  updateScreen();

  int key = getKey();

  while (key != SDLK_RETURN && key != SDLK_ESCAPE) {
    key = getKey();
  }

  switch (key) {
  case SDLK_RETURN:
    playing = true;
    actualPiece = rand() % 7;

    setNewPieceInField(actualPiece);
    break;
  case SDLK_ESCAPE:
    playing = false;
    exit(0);
  }
}
