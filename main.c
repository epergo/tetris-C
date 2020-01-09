#include "pieces.inl"
#include "primlib.h"

#include <stdbool.h>
#include <stdlib.h>

/* Constants */
#define DELAY 16
#define F_COLS 10
#define F_ROWS 20
#define VELOCITY 0.5

/* Function headers*/
void drawField();

void drawControlsAndScore(int, int);

void initField();

void drawNextPiece();

int setNewPieceInField(int);

void movePiece(int);

void moveLeft();

void moveRight();

int moveDown();

void rotatePiece();

void checkIfLine();

void endGame();

/* Global variables */
int field[F_COLS][F_ROWS];
int drawGrill;
int playing; /* 1 keep playing, 0 exit */
int actualC;
int actualR;
double actualY;
int actualPiece;
int nextPiece;
int rotation;
int score;

int main(int argc, char *args[]) {
  /* Init the seed for random numbers */
  srand(time(NULL));

  /* Init graphics */
  if (initGraph("It's Tetris!")) {
    exit(3);
  }

  /* Init variables */
  playing = 1;
  drawGrill = -1;
  rotation = 0;
  score = 0;

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
      playing = 0;
    } else {
      movePiece(keyPressed);
      drawField();
      drawNextPiece();

      updateScreen();
      SDL_Delay(DELAY);
    }
  } while (playing == 1);

  freeResources();

  return 0;
}

/* ********************** DRAW FUNCTIONS ********************** */
void drawField() {
  int padding = 30;
  int increment = 15;
  int squareWidth = 20;
  double screenWidthPadding = (screenWidth() - (F_COLS * squareWidth)) / 2;
  double screenHeightPadding = (screenHeight() - (F_ROWS * squareWidth)) / 2;

  /* Decoration of the field */
  rect(screenWidthPadding - 5, screenHeightPadding - 5, screenWidthPadding + 20 * F_COLS + 4,
       screenHeightPadding + 20 * F_ROWS + 3, BLUE);
  rect(screenWidthPadding - 3, screenHeightPadding - 3, screenWidthPadding + 20 * F_COLS + 6,
       screenHeightPadding + 20 * F_ROWS + 6, BLUE);

  drawControlsAndScore(padding, increment);

  for (int counter_c = 0; counter_c < F_COLS; counter_c++) {
    for (int counter_r = 0; counter_r < F_ROWS; counter_r++) {
      int x1 = screenWidthPadding + counter_c * 20;
      int x2 = screenWidthPadding + (counter_c + 1) * 20;
      int y1 = screenHeightPadding + counter_r * 20;
      int y2 = screenHeightPadding + (counter_r + 1) * 20;

      if (field[counter_c][counter_r] == 0) {
        if (drawGrill == 1)
          rectRGB(x1, y1, x2, y2, 92, 92, 92);
      } else {
        if (field[counter_c][counter_r] == 2) {
          filledRect(x1, y1, x2, y2, YELLOW);
          rect(x1, y1, x2, y2, BLACK);
        } else if (field[counter_c][counter_r] == 1) {
          filledRect(x1, y1, x2, y2, GREEN);
          rect(x1, y1, x2, y2, BLACK);
        } else if (field[counter_c][counter_r] == 3) {
          filledRect(x1, y1, x2, y2, RED);
          rect(x1, y1, x2, y2, WHITE);
        }
      }
    }
  }
}

void drawControlsAndScore(int padding, int increment) {
  double screenWidthPadding;
  double screenHeightPadding;
  int squareWidth;
  char str[100];

  squareWidth = 20;
  screenWidthPadding = (screenWidth() - (F_COLS * squareWidth)) / 2;
  screenHeightPadding = (screenHeight() - (F_ROWS * squareWidth)) / 2;

  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding,
          "CONTROLS:", YELLOW);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "1", RED);
  textOut(screenWidth() - screenWidthPadding + 33, screenHeightPadding + padding,
          "-> draw a grill to", YELLOW);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "help you",
          YELLOW);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "left arrow",
          RED);
  textOut(screenWidth() - screenWidthPadding + 115, screenHeightPadding + padding, "-> move the",
          YELLOW);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding,
          "piece to the left", YELLOW);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "right arrow",
          RED);
  textOut(screenWidth() - screenWidthPadding + 113, screenHeightPadding + padding, "-> move the",
          YELLOW);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding,
          "piece to the right", YELLOW);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "space ", RED);
  textOut(screenWidth() - screenWidthPadding + 73, screenHeightPadding + padding,
          "-> piece to floor", YELLOW);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "enter", RED);
  textOut(screenWidth() - screenWidthPadding + 65, screenHeightPadding + padding, "-> rotate piece",
          YELLOW);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "ESC", RED);
  textOut(screenWidth() - screenWidthPadding + 49, screenHeightPadding + padding,
          "-> exits the game", YELLOW);
  padding += increment;

  sprintf(str, "%d", score);

  textOut(50, screenHeightPadding + padding, "SCORE: ", YELLOW);
  textOut(50 + 48, screenHeightPadding + padding, str, YELLOW);
}

void drawNextPiece() {
  int squareWidth = 20;

  double screenWidthPadding = (screenWidth() - (F_COLS * squareWidth)) / 8;
  double screenHeightPadding = (screenHeight() - (F_ROWS * squareWidth)) / 2;

  textOut(screenWidthPadding, screenHeightPadding + 5, "NEXT PIECE:", YELLOW);

  rect(screenWidthPadding - 5, screenHeightPadding - 5, screenWidthPadding + 20 * 4 + 10,
       screenHeightPadding + 20 * 6 + 5, RED);
  rect(screenWidthPadding - 3, screenHeightPadding - 3, screenWidthPadding + 20 * 4 + 13,
       screenHeightPadding + 20 * 6 + 8, RED);

  for (int counterC = 0; counterC < 4; counterC++) {
    for (int counterR = 0; counterR < 4; counterR++) {
      int x1 = screenWidthPadding + (counterC + 1) * 20;
      int x2 = screenWidthPadding + (counterC + 2) * 20;
      int y1 = screenHeightPadding + (counterR + 2) * 20;
      int y2 = screenHeightPadding + (counterR + 3) * 20;

      if (pieces[nextPiece][0][counterC][counterR] == 1) {
        filledRect(x1, y1, x2, y2, GREEN);
      } else if (pieces[nextPiece][0][counterC][counterR] == 2) {
        filledRect(x1, y1, x2, y2, YELLOW);
      }
    }
  }
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
    drawGrill = -drawGrill;
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
    playing = 0;
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
      if (insidePosition == 1 || insidePosition == 2) {
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

        if (insidePosition == 1 || insidePosition == 2) {
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

          if (insidePosition == 1 || insidePosition == 2) {
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
        if (field[actualC + counterC][actualR + counterR] == 1 ||
            field[actualC + counterC][actualR + counterR] == 2) {
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
            if (field[actualC + counterC][actualR + counterR] == 1 ||
                field[actualC + counterC][actualR + counterR] == 2) {
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
  int c;
  int r;
  int y = 99;
  int yActual = 99;
  int exit = 0;
  int isPossible = 0;

  if (rotation == 3)
    rotation = 0;
  else
    rotation += 1;

  for (c = 0; c < 4 && y == 99; c++) {
    for (r = 0; r < 4; r++) {
      int pos = pieces[actualPiece][rotation][c][r];

      if (pos == 2) {
        y = c;
        break;
      }
    }
  }

  for (c = 0; c < 4 && isPossible == 0; c++) {
    for (r = 0; r < 4; r++) {
      if (actualC + c >= 0 && actualC + c < F_COLS && actualR + r >= 0 &&
          actualR + r < F_ROWS - 1) {
        pos_f = field[actualC + c][actualR + r];
        int pos = pieces[actualPiece][rotation][c][r];

        if (pos_f == 3 && pos != 0) {
          isPossible = 1;
          break;
        }

        if (pos_f == 2) {
          yActual = c;
        }

      } else {
        if (pos != 0) {
          isPossible = 1;
          break;
        }
      }
    }
  }

  if (actualC + yActual - y < 0 || isPossible == 1) {
    /* Impossible to do the rotation */
    rotation = rotation - 1;
  } else {
    for (c = 3; c >= 0 && exit != 1; c--) {
      for (r = 3; r >= 0 && exit != 1; r--) {
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
  int columns;
  int rows;

  int line;

  line = 1; /* 1 line complete, 0 no*/

  for (rows = F_ROWS - 1; rows >= 0; rows--) {
    line = 1;
    for (columns = 0; columns < F_COLS; columns++) {
      if (field[columns][rows] != 3) {
        line = 0;
        break;
      }
    }

    if (line == 1) {
      int rows2;
      score += 50;

      for (rows2 = rows; rows2 > 0; rows2--) {
        for (columns = 0; columns < F_COLS; columns++) {
          if (rows2 - 1 == 0) {
            field[columns][rows2 - 1] = 0;
          }

          field[columns][rows2] = field[columns][rows2 - 1];
        }
      }
      rows++;
    }
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
    playing = 1;
    actualPiece = rand() % 7;

    setNewPieceInField(actualPiece);
    break;
  case SDLK_ESCAPE:
    playing = 0;
    exit(0);
  }
}
