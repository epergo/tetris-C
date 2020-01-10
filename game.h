#define DELAY 16
#define F_COLS 10
#define F_ROWS 20
#define VELOCITY 0.5

int play();
void initField();
void movePiece(int);
void moveLeft();
void moveRight();
void rotatePiece();
void checkIfLine();
void endGame();
int setNewPieceInField(int);
int moveDown();
