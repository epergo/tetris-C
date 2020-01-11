#include "userInterface.h"
#include "primlib.h"

extern const Uint8 pieceColor[7][3];
extern const Uint8 setPieceColor[7][3];
extern const char pieces[7][4][4][4];
extern const int empty;
extern const int player;
extern const int filled;

const int squareWidth = 20;

void drawField(int field[F_COLS][F_ROWS]) {
  int screenWidthPadding = (screenWidth() - (F_COLS * SQUARE_WIDTH)) / 2;
  int screenHeightPadding = (screenHeight() - (F_ROWS * SQUARE_WIDTH)) / 2;

  /* Decoration of the field */
  rect(screenWidthPadding - 5, screenHeightPadding - 5, screenWidthPadding + 20 * F_COLS + 4,
       screenHeightPadding + 20 * F_ROWS + 3, 255, 255, 255);
  rect(screenWidthPadding - 3, screenHeightPadding - 3, screenWidthPadding + 20 * F_COLS + 6,
       screenHeightPadding + 20 * F_ROWS + 6, 255, 255, 255);

  for (int counter_c = 0; counter_c < F_COLS; counter_c++) {
    for (int counter_r = 0; counter_r < F_ROWS; counter_r++) {
      int x1 = screenWidthPadding + counter_c * 20;
      int x2 = screenWidthPadding + (counter_c + 1) * 20;
      int y1 = screenHeightPadding + counter_r * 20;
      int y2 = screenHeightPadding + (counter_r + 1) * 20;

      rect(x1, y1, x2, y2, 23, 23, 23);
      if (whatIsInside(field[counter_c][counter_r]) == player) {
        Uint8 piece = field[counter_c][counter_r] - 1;
        filledRect(x1, y1, x2, y2, pieceColor[piece][0], pieceColor[piece][1],
                   pieceColor[piece][2]);

        rect(x1, y1, x2, y2, 26, 26, 26);
      } else if (whatIsInside(field[counter_c][counter_r]) == filled) {
        Uint8 piece = (field[counter_c][counter_r] / 100) - 1;
        filledRect(x1, y1, x2, y2, setPieceColor[piece][0], setPieceColor[piece][1],
                   setPieceColor[piece][2]);

        rect(x1, y1, x2, y2, 26, 26, 26);
      }
    }
  }
}

void drawScore(int score) {
  int screenWidthPadding = (screenWidth() - (F_COLS * squareWidth)) / 2;
  int screenHeightPadding = (screenHeight() - ((F_ROWS + 2) * squareWidth)) / 2;

  char str[100];
  sprintf(str, "SCORE %d", score);

  textOut(screenWidthPadding - 5, screenHeightPadding, str, 205, 47, 2);
}
void drawControls() {
  int padding = 30;
  int increment = 15;
  int screenWidthPadding = (screenWidth() - (F_COLS * squareWidth)) / 2;
  int screenHeightPadding = (screenHeight() - (F_ROWS * squareWidth)) / 2;

  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "CONTROLS:", 0, 0,
          0);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "left arrow", 0,
          0, 0);
  textOut(screenWidth() - screenWidthPadding + 115, screenHeightPadding + padding, "-> move the", 0,
          0, 0);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding,
          "piece to the left", 0, 0, 0);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "right arrow", 0,
          0, 0);
  textOut(screenWidth() - screenWidthPadding + 113, screenHeightPadding + padding, "-> move the", 0,
          0, 0);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding,
          "piece to the right", 0, 0, 0);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "space ", 0, 0,
          0);
  textOut(screenWidth() - screenWidthPadding + 73, screenHeightPadding + padding,
          "-> piece to floor", 0, 0, 0);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "enter", 0, 0, 0);
  textOut(screenWidth() - screenWidthPadding + 65, screenHeightPadding + padding, "-> rotate piece",
          0, 0, 0);
  padding += increment;
  textOut(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "ESC", 0, 0, 0);
  textOut(screenWidth() - screenWidthPadding + 49, screenHeightPadding + padding,
          "-> exits the game", 0, 0, 0);
  padding += increment;
}

void drawNextPiece(int nextPiece) {
  int screenWidthPadding = (screenWidth() - (F_COLS * SQUARE_WIDTH)) / 3;
  int screenHeightPadding = (screenHeight() - (F_ROWS * SQUARE_WIDTH)) / 2;

  rect(screenWidthPadding - 5, screenHeightPadding - 5, screenWidthPadding + 20 * 4 + 10,
       screenHeightPadding + 20 * 6 + 5, 255, 255, 255);
  rect(screenWidthPadding - 3, screenHeightPadding - 3, screenWidthPadding + 20 * 4 + 13,
       screenHeightPadding + 20 * 6 + 8, 255, 255, 255);

  for (int counterC = 0; counterC < 4; counterC++) {
    for (int counterR = 0; counterR < 4; counterR++) {
      int x1 = screenWidthPadding + (counterC + 1) * 20;
      int x2 = screenWidthPadding + (counterC + 2) * 20;
      int y1 = screenHeightPadding + (counterR + 2) * 20;
      int y2 = screenHeightPadding + (counterR + 3) * 20;

      if (whatIsInside(pieces[nextPiece][0][counterC][counterR]) == player) {
        filledRect(x1, y1, x2, y2, pieceColor[nextPiece][0], pieceColor[nextPiece][1],
                   pieceColor[nextPiece][2]);

        rect(x1, y1, x2, y2, 26, 26, 26);
      }
    }
  }
}

void drawEndGame(int score) {
  char str[100];
  sprintf(str, "%d", score);

  textOut(screenWidth() / 3 + 145, screenHeight() / 2 - 25, "SCORE", 0, 0, 0);
  textOut(screenWidth() / 3 + 215, screenHeight() / 2 - 25, str, 0, 0, 0);

  textOut(screenWidth() / 3 + 25, screenHeight() / 2 - 25, "GAME OVER", 0, 0, 0);
  textOut(screenWidth() / 3 - 25, screenHeight() / 2, "PRESS ENTER TO PLAY AGAIN OR ESC TO EXIT", 0,
          0, 0);
}
