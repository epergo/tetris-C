#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include <SDL/SDL.h>

#include <SDL/SDL_gfxPrimitives.h>

#include "primlib.h"


static SDL_Surface *screen;
static Uint32 colors[COLOR_MAX] = {0x000000ff, 
                               0xff0000ff, 0x00ff00ff, 0x0000ffff,
                               0x00ffffff, 0xff00ffff, 0xffff00ff,
                               0xffffffff};


void pixel(int x, int y, enum color c)
{
	pixelColor(screen, x,y,colors[c]);
}

void line(int x1, int y1, int x2, int y2, enum color c)
{
	assert(c<COLOR_MAX);
	lineColor(screen, x1,y1,x2,y2,colors[c]);
}


void rect(int x1, int y1, int x2, int y2, enum color c)
{
	assert(c<COLOR_MAX);
	rectangleColor(screen, x1,y1,x2,y2,colors[c]);
}

void filledRect(int x1, int y1, int x2, int y2, enum color c)
{
	assert(c<COLOR_MAX);
	boxColor(screen, x1,y1,x2,y2,colors[c]);
}


void circle(int x, int y, int r, enum color c)
{
	assert(c<COLOR_MAX);
	circleColor(screen, x,y,r,colors[c]);
}

void filledCircle(int x, int y, int r, enum color c)
{
	assert(c<COLOR_MAX);
	filledCircleColor(screen, x,y,r,colors[c]);

}

void updateScreen()
{
	SDL_Flip(screen);
}


int screenWidth()
{
	return screen->w;
}

int screenHeight()
{
	return screen->h;
}


void textout(int x, int y, char *s, enum color c)
{
	assert(c<COLOR_MAX);
	stringColor(screen, x, y, s, colors[c]);
} 

int pollkey()
{
	SDL_Event event;
	while ( SDL_PollEvent(&event) ) 
	{
		switch (event.type) {
			case SDL_KEYDOWN:
				return event.key.keysym.sym;
			case SDL_QUIT:
				exit(3);
		}
	}
	return -1;
}

int getkey()
{
	SDL_Event event;
	while(1)
	{
	  SDL_WaitEvent(&event);
	  if(event.type==SDL_KEYDOWN)
		  break;
      if(event.type==SDL_QUIT)
		  exit(3);
	};
	return event.key.keysym.sym;
}

int isKeyDown(int key)
{
 Uint8* keytable;
 int numkeys;
 SDL_PumpEvents();
 keytable=SDL_GetKeyState(&numkeys);
 assert(key<numkeys);
 return keytable[key];
}

int initGraph()
{
	const SDL_VideoInfo *info;
	Uint8  video_bpp;
	Uint32 videoflags;
        
        if(screen)
        {
		fprintf(stderr,"initGraph called twice\n");
		return 1;
	}
	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	/* Alpha blending doesn't work well at 8-bit color */
	info = SDL_GetVideoInfo();
	if ( info->vfmt->BitsPerPixel > 8 ) {
		video_bpp = info->vfmt->BitsPerPixel;
	} else {
		video_bpp = 16;
	}
	videoflags = SDL_SWSURFACE | SDL_DOUBLEBUF;
	
	/* Set 640x480 video mode */
	if ( (screen=SDL_SetVideoMode(640,480,video_bpp,videoflags)) == NULL ) {
		fprintf(stderr, "Couldn't set %ix%i video mode: %s\n",640,480,SDL_GetError());
		return 2;
	}
    return 0;
}
