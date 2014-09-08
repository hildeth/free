



#include <SDL.h>
#include <stdio.h>

SDL_Window *screen;
SDL_Cursor *cursor;

static void create_cursor()
{
	Uint8 data[4*8] = { 0x38,
			    0x44,
			    0x92,
			    0xAA,
			    0x92,
			    0x44,
			    0x38,
			    0x00 };
	Uint8 mask[4*8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
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
	screen = SDL_CreateWindow("Downgrade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
	if (screen == NULL)
	{
        fprintf(stderr, "Couldn't get 640x480 window: %s\n", SDL_GetError());
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
