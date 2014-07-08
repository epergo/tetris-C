#include "primlib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "pieces.inl"

/* Constants */
#define DELAY 16
#define F_COLS 10
#define F_ROWS 20
#define VELOCITY 0.5

/* Function headers*/
void drawField(int);
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

/* Main function */
int main()
{
	/* Local variables */
	int keyPressed;

	/* Init the seed for random numbers */
	srand(time(NULL));

	/* Init graphics */
	if(initGraph())
		exit(3);

	/* Init variables */
	playing = 1;
	drawGrill = -1;
	rotation = 0;
	score = 0;

	/* Puts all the positions to 0 */	  
	initField();
	
	/* Getting the first piece */
	actualPiece = rand()%7;
	nextPiece = rand()%7;
	setNewPieceInField(actualPiece);
	
	do
	{
		/* Clear screen */
		filledRect(0, 0, screenWidth()-1, screenHeight()-1, BLACK);
		
		keyPressed = pollkey();
		
		movePiece(keyPressed);
		
		drawField(drawGrill);
		drawNextPiece();

		updateScreen();
		SDL_Delay(DELAY);
	}while(playing == 1);
	

	return 0;
}

/* ********************** DRAW FUNCTIONS ********************** */
void drawField(int drawGrill)
{
	double screenWidthPadding;
	double screenHeightPadding;
	int squareWidth;
	int counter_r;
	int counter_c;
	int padding;
	int increment;
	
	squareWidth = 20;
	screenWidthPadding = (screenWidth() - (F_COLS * squareWidth)) / 2;
	screenHeightPadding = (screenHeight() - (F_ROWS * squareWidth)) / 2;

	/* Decoration of the field */
	rect(screenWidthPadding - 5, screenHeightPadding - 5, screenWidthPadding + 20 * F_COLS + 4, screenHeightPadding + 20 * F_ROWS + 3, BLUE);
	rect(screenWidthPadding - 3, screenHeightPadding - 3, screenWidthPadding + 20 * F_COLS + 6, screenHeightPadding + 20 * F_ROWS + 6, BLUE);

	padding = 30;
	increment = 15;
	
	drawControlsAndScore(padding, increment);
	
	for(counter_c = 0; counter_c < F_COLS; counter_c++)
	{
		for(counter_r = 0; counter_r < F_ROWS; counter_r++)
		{
			int x1 = screenWidthPadding + counter_c * 20;
			int x2 = screenWidthPadding + (counter_c + 1) * 20;
			int y1 = screenHeightPadding + counter_r * 20;
			int y2 = screenHeightPadding + (counter_r + 1) * 20;
			
			if(field[counter_c][counter_r] == 0)
			{
				if(drawGrill == 1)
					rect(x1, y1, x2, y2, WHITE);
			}
			else
			{
				if(field[counter_c][counter_r] == 2)
				{
					filledRect(x1, y1, x2, y2, YELLOW);
					rect(x1, y1, x2, y2, BLACK);
				}
				else if(field[counter_c][counter_r] == 1)
				{
					filledRect(x1, y1, x2, y2, GREEN);
					rect(x1, y1, x2, y2, BLACK);
				}
				else if(field[counter_c][counter_r] == 3)
				{
					filledRect(x1, y1, x2, y2, RED);
				}
					
			}
		}
	}
}

void drawControlsAndScore(int padding, int increment)
{
	double screenWidthPadding;
	double screenHeightPadding;
	int squareWidth;
	char str[100]; 
	
	squareWidth = 20;	
	screenWidthPadding = (screenWidth() - (F_COLS * squareWidth)) / 2;
	screenHeightPadding = (screenHeight() - (F_ROWS * squareWidth)) / 2;

	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "CONTROLS:", YELLOW);
	padding += increment;
	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "1", RED);
	textout(screenWidth() - screenWidthPadding + 33, screenHeightPadding + padding, "-> draw a grill to", YELLOW);
	padding += increment;
	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "help you", YELLOW);
	padding += increment;
	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "left arrow", RED);
	textout(screenWidth() - screenWidthPadding + 115, screenHeightPadding + padding, "-> move the", YELLOW);
	padding += increment;
	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "piece to the left", YELLOW);
	padding += increment;
	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "right arrow", RED);
	textout(screenWidth() - screenWidthPadding + 113, screenHeightPadding + padding, "-> move the", YELLOW);
	padding += increment;
	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "piece to the right", YELLOW);
	padding += increment;
	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "space ", RED);
	textout(screenWidth() - screenWidthPadding + 73, screenHeightPadding + padding, "-> piece to floor", YELLOW);
	padding += increment;
	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "enter", RED);
	textout(screenWidth() - screenWidthPadding + 65, screenHeightPadding + padding, "-> rotate piece", YELLOW);
	padding += increment;
	textout(screenWidth() - screenWidthPadding + 25, screenHeightPadding + padding, "ESC", RED);
	textout(screenWidth() - screenWidthPadding + 49, screenHeightPadding + padding, "-> exits the game", YELLOW);
	padding += increment;
	
	sprintf(str, "%d", score);
	
	textout(50, screenHeightPadding + padding, "SCORE: ", YELLOW);
	textout(50 + 48, screenHeightPadding + padding, str, YELLOW);
}

void drawNextPiece()
{
	double screenWidthPadding;
	double screenHeightPadding;
	int squareWidth;
	int counterC;
	int counterR;
	
	int x1;
	int x2;
	int y1;
	int y2;
	
	squareWidth = 20;	
	screenWidthPadding = (screenWidth() - (F_COLS * squareWidth)) / 8;
	screenHeightPadding = (screenHeight() - (F_ROWS * squareWidth)) / 2;
	
	textout(screenWidthPadding, screenHeightPadding + 5, "NEXT PIECE:", YELLOW);
	
	rect(screenWidthPadding - 5, screenHeightPadding - 5, screenWidthPadding + 20 * 4 + 10, screenHeightPadding + 20 * 6 + 5, RED);
	rect(screenWidthPadding - 3, screenHeightPadding - 3, screenWidthPadding + 20 * 4 + 13, screenHeightPadding + 20 * 6 + 8, RED);

	for(counterC = 0; counterC < 4; counterC++)
	{
		for(counterR = 0; counterR < 4; counterR++)
		{
			x1 = screenWidthPadding + (counterC + 1) * 20;
			x2 = screenWidthPadding + (counterC + 2) * 20;
			y1 = screenHeightPadding + (counterR + 2) * 20;
			y2 = screenHeightPadding + (counterR + 3) * 20;
			
			if(pieces[nextPiece][0][counterC][counterR] == 1)
			{
				filledRect(x1, y1, x2, y2, GREEN);
			}else if(pieces[nextPiece][0][counterC][counterR] == 2)
			{
				filledRect(x1, y1, x2, y2, YELLOW);
			}
			rect(x1, y1, x2, y2, BLACK);
		}
	}
}

/* INIT FIELD FUNCTION */
void initField()
{
	int counter_c;
	for(counter_c = 0; counter_c < F_COLS; counter_c++)
	{
		int counter_f;
		for(counter_f = 0; counter_f < F_ROWS; counter_f++)
		{
			field[counter_c][counter_f] = 0;
		}
	}
}

/* LOGIC FUNCTIONS */
int setNewPieceInField(int type)
{
	int counterC;
	int counterR;
	
	actualC = 3;
	actualR = 0;
	actualY = 0;
	rotation = 0;
	
	for(counterC = 0; counterC < 4; counterC++)
	{
		for(counterR = 0; counterR < 4; counterR++)
		{
			if(field[actualC + counterC][counterR] == 0)
			{
				field[actualC + counterC][counterR] = pieces[type][rotation][counterC][counterR];
			}else if(pieces[type][rotation][counterC][counterR] != 0)
				return -1;
		}
	}
	
	return 0;
}

void movePiece(int key)
{
	int wait;

	switch(key)
	{
		case SDLK_1:
			drawGrill = -drawGrill;
			break;
		case SDLK_LEFT:
			moveLeft();
			break;
		case SDLK_RIGHT:
			moveRight();
			break;
		case SDLK_SPACE:
			do
			{
				wait = moveDown();
			}
			while(wait != 1);
			break;
		case SDLK_RETURN:
			rotatePiece();
			break;
		case SDLK_ESCAPE:
			playing = 0;
			exit(0);
			break;
		default:
			moveDown();
			break;
	}
	
}

void moveLeft()
{
	int counterC;
	int counterR;
	int isPosible;
	
	isPosible = 1; /* 1 is posible, 0 no */
	
	for(counterC = 0; counterC < 4 && isPosible == 1; counterC++)
	{
		for(counterR = 0; counterR < 4 && isPosible == 1; counterR++)
		{
			if(actualC + counterC >= 0 && actualC + counterC < F_COLS)
			{
				int insidePosition;
				int insidePositionLeft;
				
				insidePosition = field[actualC + counterC][actualR + counterR];
			
				if(insidePosition == 1 || insidePosition == 2)
				{
					if(actualC + counterC > 0)
					{
						insidePositionLeft = field[actualC + counterC - 1][actualR + counterR];
					
						if(insidePositionLeft != 3)
						{
							/* OK */
						}
						else
						{
							isPosible = 0;
							break;
						}
					}
					else
					{
						isPosible = 0;
						break;
					}
				}
			}
		}
	}
	
	if(isPosible == 1)
	{
		for(counterC = 0; counterC < 4; counterC++)
		{
			for(counterR = 0; counterR < 4; counterR++)
			{
				if(actualC + counterC >= 0 && actualC + counterC < F_COLS)
				{
					int insidePosition;
					int insidePositionRight;
				
					insidePosition = field[actualC + counterC][actualR + counterR];
			
					if(insidePosition == 1 || insidePosition == 2)
					{
						if(actualC + counterC > 0 && actualC + counterC < F_COLS)
						{
							field[actualC + counterC - 1][actualR + counterR] = insidePosition;
							
							if(actualC + counterC + 1 < F_COLS)
							{
								insidePositionRight = field[actualC + counterC + 1][actualR + counterR];
								
								if(insidePositionRight == 0 || insidePositionRight == 3)
								{
									field[actualC + counterC][actualR + counterR] = 0;
								}
							}
							else
							{
								field[actualC + counterC][actualR + counterR] = 0;
							}
							
						}
					}
				}

			}
		}
		
		actualC -= 1;
	}		
	
	moveDown();
}

void moveRight()
{
	int counterC;
	int counterR;
	int isPosible;
	
	isPosible = 1; /* 1 is posible, 0 no */
	
	for(counterC = 0; counterC < 4 && isPosible == 1; counterC++)
	{
		for(counterR = 0; counterR < 4 && isPosible == 1; counterR++)
		{
			if(actualC + counterC < F_COLS)
			{
				int insidePosition;
				int insidePositionRight;
				
				insidePosition = field[actualC + counterC][actualR + counterR];
			
				if(insidePosition == 1 || insidePosition == 2)
				{
					if(actualC + counterC + 1 < F_COLS)
					{
						insidePositionRight = field[actualC + counterC + 1][actualR + counterR];
					
						if(insidePositionRight != 3)
						{
							/* OK */
						}
						else
						{
							isPosible = 0;
							break;
						}
					}
					else
					{
						isPosible = 0;
						break;
					}
				}
			}
		}
	}
	
	if(isPosible == 1)
	{
		for(counterC = 3; counterC >= 0; counterC--)
		{
			for(counterR = 0; counterR < 4; counterR++)
			{
				if(actualC + counterC  + 1 < F_COLS)
				{
					int insidePosition;
					int insidePositionLeft;
				
					insidePosition = field[actualC + counterC][actualR + counterR];
			
					if(insidePosition == 1 || insidePosition == 2)
					{
						if(actualC + counterC + 1 < F_COLS)
						{
							field[actualC + counterC + 1][actualR + counterR] = insidePosition;
							
							if(actualC + counterC > 0)
							{
								insidePositionLeft = field[actualC + counterC - 1][actualR + counterR];
								
								if(insidePositionLeft == 0 || insidePositionLeft == 3)
								{
									field[actualC + counterC][actualR + counterR] = 0;
								}
							}
							else
							{
								field[actualC + counterC][actualR + counterR] = 0;
							}
							
						}
					}
				}

			}
		}
		
		actualC += 1;
		printf("%i\n", actualC);
	}		
	
	moveDown();
}

int moveDown()
{
	
	int counterC;
	int counterR;
	int isPosible;
	
	

	isPosible = 1;

	for(counterC = 0; counterC < 4 && isPosible == 1; counterC++)
	{
		for(counterR = 0; counterR < 4; counterR++)
		{
			if(actualR + counterR < F_ROWS && actualC + counterC < F_COLS)
			{
				if(field[actualC + counterC][actualR + counterR] == 1 || field[actualC + counterC][actualR + counterR] == 2)
				{
					if(actualR + counterR > F_ROWS - 2)
					{
						isPosible = 0;
						break;
					}
					else if(field[actualC + counterC][actualR + counterR + 1] == 3)
					{
						isPosible = 0;
						break;
					}
				}
			}
		}
	}
	
	actualY += VELOCITY;
	
	if(actualY >= actualR * 20)
	{
		actualY = actualR * 20;
		
		if(isPosible == 1)
		{
			for(counterC = 0; counterC < 4; counterC++)
			{
				for(counterR = 3; counterR >= 0; counterR--)
				{
					if(actualR + counterR + 1 < F_ROWS && actualC + counterC < F_COLS)
					{
						if(field[actualC + counterC][actualR + counterR] != 3 && field[actualC + counterC][actualR + counterR] != 0)
						{
							field[actualC + counterC][actualR + counterR + 1] = field[actualC + counterC][actualR + counterR];
					
							if(actualR + counterR - 1 < 0 || field[actualC + counterC][actualR + counterR - 1] == 3)
								field[actualC + counterC][actualR + counterR] = 0;
							else
								field[actualC + counterC][actualR + counterR] = field[actualC + counterC][actualR + counterR - 1];
						}
					}
				}
			}
			actualR += 1;
		}
		else
		{
			for(counterC = 0; counterC < 4; counterC++)
			{
				for(counterR = 0; counterR < 4 && actualR + counterR < F_ROWS; counterR++)
				{
					if(actualR + counterR< F_ROWS && actualC + counterC < F_COLS)
					{
						if(field[actualC + counterC][actualR + counterR] == 1 || field[actualC + counterC][actualR + counterR] == 2)
						{
							field[actualC + counterC][actualR + counterR] = 3;
						}	
					}
				
				}
			}
		
			actualPiece = nextPiece;
			nextPiece = rand()%7;
		
			checkIfLine();
			
		
			if(setNewPieceInField(actualPiece) == -1)
			{
				endGame();
			}
		
			return 1;
		}
	}
	return 0;
	
}

void rotatePiece()
{
	int pos;
	int pos_f;
	int c;
	int r;
	int y;
	int yActual;
	int exit;
	int isPosible;
	
	isPosible = 0;
	
	exit = 0;
	y = 99;
	yActual = 99;
	
	if(rotation == 3)
		rotation = 0;
	else
		rotation += 1;
	
	for(c = 0; c < 4 && y == 99; c++)
	{
		for(r = 0; r < 4; r++)
		{
			pos = pieces[actualPiece][rotation][c][r];
			
			if(pos == 2)
			{
				/*printf("Encuentra el centro\n");*/
				y = c;
				break;
			}
		}
	}
	
	for(c = 0; c < 4 && isPosible == 0; c++)
	{
		for(r = 0; r < 4; r++)
		{
			if(actualC + c >= 0 && actualC + c < F_COLS && actualR + r >= 0 && actualR + r < F_ROWS - 1)
			{
				pos_f = field[actualC + c][actualR + r];
				pos = pieces[actualPiece][rotation][c][r];
				
				printf("posf: %i ---- pos: %i\n", pos_f, pos);
				
				if(pos_f == 3 && pos != 0)
				{
					isPosible = 1;
					printf("entra, isposible = %i\n", isPosible);
					break;
				}
				
				if(pos_f == 2)
				{
					yActual = c;
				}
				
			}
			else
			{
				if(pos != 0)
				{	
					isPosible = 1;
					break;			
				}
			}

		}
	}
	
	printf("yActual(%i) - y(%i) = %i\n", yActual, y, yActual - y);
	
	printf("isposible %i\n", isPosible);
	if(actualC + yActual - y < 0 || isPosible == 1)
	{
		/* Imposible to do the rotation */
		rotation = rotation - 1;
	}
	else
	{
	printf("isposible %i AUN ASI ENTRA\n", isPosible);
		for(c = 3; c >= 0 && exit != 1 && isPosible == 0; c--)
		{
			for(r = 3; r >= 0 && exit != 1; r--)
			{
				pos = pieces[actualPiece][rotation][c][r];
				
				if(actualC + c >= 0 && actualC + c < F_COLS)
				{
					if(actualR + r >= 0 && actualR + r < F_ROWS - 1)
					{
						pos_f = field[actualC + c][actualR + r];
						
						if(pos_f != 3)
						{
							/*printf("Efectua el cambio\n");*/
							field[actualC + c][actualR + r] = pos;
						}
						else
						{
							if(pos != 0)
								exit = 1;
						}
					}
					else
					{
						if(pos != 0)
							exit = 1;
					}
				}
				else
				{
					if(pos != 0)
						exit = 1;
				}
			}
		}
		
		if(exit == 1)
			rotation = rotation - 1;
		
	}
	
}

void checkIfLine()
{
	int columns;
	int rows;
	
	int line;
	
	line = 1; /* 1 line complete, 0 no*/
	
	for(rows = F_ROWS - 1; rows >= 0; rows--)
	{
		line = 1;
		for(columns = 0; columns < F_COLS; columns++)
		{
			if(field[columns][rows] != 3)
			{
				line = 0;
				break;
			}
		}
		
		if(line == 1)
		{
			int rows2;
			score += 50;
			
			for(rows2 = rows; rows2 > 0; rows2--)
			{
				for(columns = 0; columns < F_COLS; columns++)
				{
					if(rows2 - 1 == 0 )
					{
						field[columns][rows2 - 1] = 0;
					}
					
					field[columns][rows2] = field[columns][rows2 - 1];
					
				}
			}
			rows++;
		}
	}
}

void endGame()
{
	int key;
	char str[100];
	
	initField();

	sprintf(str, "%d", score);
	
	textout(screenWidth() / 3 + 145, screenHeight() / 2 - 25, "SCORE", YELLOW);
	textout(screenWidth() / 3 + 215, screenHeight() / 2 - 25, str, YELLOW);

	textout(screenWidth() / 3 + 25, screenHeight() / 2 - 25, "GAME OVER", RED);
	textout(screenWidth() / 3 - 25, screenHeight() / 2 , "PRESS ENTER TO PLAY AGAIN OR ESC TO EXIT", RED);
	updateScreen();
	
	key = getkey();
	
	while(key != SDLK_RETURN && key != SDLK_ESCAPE)
	{
		key = getkey();
	}
	
	switch(key)
	{
		case SDLK_RETURN:
			playing = 1;
			actualPiece = rand()%7;
			
			setNewPieceInField(actualPiece);
			break;
		case SDLK_ESCAPE:
			playing = 0;
			exit(0);
			break;
	}
}
