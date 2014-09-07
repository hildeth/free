#include "SDL.h"
#include <stdio.h>

SDL_Surface *screen;
SDL_Cursor *cursor;

static void create_cursor()
{
	Uint8 data[8*8] = { 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
						0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
						0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
						0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
						0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
						0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
						0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	Uint8 mask[8*8] = { 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
						0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
						0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00,
						0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
						0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	cursor = SDL_CreateCursor(data, mask, 8, 8, 3, 3);
	SDL_SetCursor(cursor);
}

static void display_init()
{
//	printf("Initializing SDL\n");
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
		exit(1);
	}
//	printf("SDL initialized.\n");
	atexit(SDL_Quit);

    /*
     * Initialize the display in a 640x480 8-bit palettized mode,
     * requesting a software surface
     */
    screen = SDL_SetVideoMode(640, 480, 8, SDL_HWSURFACE);
	if (screen == NULL)
	{
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n", SDL_GetError());
        exit(1);
	}
	create_cursor();
}

static void event_loop()
{
	int done = false;
	while (! done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				done = true;
		}
		SDL_Delay(10); // Don't be hyper.
	}
}

int main()
{
	display_init();
	event_loop();
	exit(0);
}