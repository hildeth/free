#include "SDL.h"   /* All SDL App's need this */
#include <stdio.h>

unsigned int Fred(unsigned int boo, void* stupid)
{
	SDL_Surface *freen;
	freen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
	++*(int*)stupid;
	return 0;
}

int main()
{    
	SDL_Surface *screen;
	int myint=0;

    /* Initialize the SDL library */
    if( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Clean up on exit */
    atexit(SDL_Quit);
    
    /*
     * Initialize the display in a 640x480 8-bit palettized mode,
     * requesting a software surface
     */
    screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
	if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
                        SDL_GetError());
        exit(1);
		}
	SDL_AddTimer(10000,Fred, &myint);
	SDL_Delay(10100);
	printf("%d\n", myint);
	SDL_Delay(100000);
}

