all: tetris

tetris: tetris.o primlib.o
	gcc -g $^ -o $@ -lm -lSDL -lpthread -lSDL_gfx

.c.o: 
	gcc -g -Wall -pedantic -Werror -c -D_REENTRANT $<

primlib.o: primlib.c primlib.h

tetris.o: tetris.c primlib.h

clean:
	-rm primlib.o tetris.o tetris
