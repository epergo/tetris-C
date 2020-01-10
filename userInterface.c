#include "userInterface.h"
#include "primlib.h"
#include <stdbool.h>

void drawField(int field[F_COLS][F_ROWS], bool drawGrill) {
  int screenWidthPadding = (screenWidth() - (F_COLS * SQUARE_WIDTH)) / 2;
  int screenHeightPadding = (screenHeight() - (F_ROWS * SQUARE_WIDTH)) / 2;

  /* Decoration of the field */
  rect(screenWidthPadding - 5, screenHeightPadding - 5, screenWidthPadding + 20 * F_COLS + 4,
       screenHeightPadding + 20 * F_ROWS + 3, BLUE);
  rect(screenWidthPadding - 3, screenHeightPadding - 3, screenWidthPadding + 20 * F_COLS + 6,
       screenHeightPadding + 20 * F_ROWS + 6, BLUE);

  for (int counter_c = 0; counter_c < F_COLS; counter_c++) {
    for (int counter_r = 0; counter_r < F_ROWS; counter_r++) {
      int x1 = screenWidthPadding + counter_c * 20;
      int x2 = screenWidthPadding + (counter_c + 1) * 20;
      int y1 = screenHeightPadding + counter_r * 20;
      int y2 = screenHeightPadding + (counter_r + 1) * 20;

      if (field[counter_c][counter_r] == 0) {
        if (drawGrill) {
          rectRGB(x1, y1, x2, y2, 92, 92, 92);
        }
      } else {
        if (field[counter_c][counter_r] == 1) {
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

void drawControlsAndScore(int score) {
  int padding = 30;
  int increment = 15;
  int squareWidth = 20;
  int screenWidthPadding = (screenWidth() - (F_COLS * squareWidth)) / 2;
  int screenHeightPadding = (screenHeight() - (F_ROWS * squareWidth)) / 2;

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

  char str[100];
  sprintf(str, "%d", score);

  textOut(50, screenHeightPadding + padding, "SCORE: ", YELLOW);
  textOut(50 + 48, screenHeightPadding + padding, str, YELLOW);
}

void drawNextPiece(char pieces[7][4][4][4], int nextPiece) {
  int screenWidthPadding = (screenWidth() - (F_COLS * SQUARE_WIDTH)) / 3;
  int screenHeightPadding = (screenHeight() - (F_ROWS * SQUARE_WIDTH)) / 2;

  rect(screenWidthPadding - 5, screenHeightPadding - 5, screenWidthPadding + 20 * 4 + 10,
       screenHeightPadding + 20 * 6 + 5, BLUE);
  rect(screenWidthPadding - 3, screenHeightPadding - 3, screenWidthPadding + 20 * 4 + 13,
       screenHeightPadding + 20 * 6 + 8, BLUE);

  for (int counterC = 0; counterC < 4; counterC++) {
    for (int counterR = 0; counterR < 4; counterR++) {
      int x1 = screenWidthPadding + (counterC + 1) * 20;
      int x2 = screenWidthPadding + (counterC + 2) * 20;
      int y1 = screenHeightPadding + (counterR + 2) * 20;
      int y2 = screenHeightPadding + (counterR + 3) * 20;

      if (pieces[nextPiece][0][counterC][counterR] == 1) {
        filledRect(x1, y1, x2, y2, GREEN);
      }
    }
  }
}
