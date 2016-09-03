/**
 * file: main.cpp
 * purpose: test sound class SDL_snd.
 * @author Dominik Haumann
 */

/**
I basically do the following steps:

// init SDL Audio:
SDL_Init(SDL_INIT_AUDIO);

// create & initialize sound class instance:
SDL_snd snd;
snd.init(22500);

// load, then play
const SndInfo* snd1 = snd.load("test.wav");
snd.play(snd1);

// load and play immediately
const SndInfo* snd2 = snd.play("test.wav");

// unload
snd.unload(snd1);

**/

#include <SDL/SDL.h>
#include "sdl_snd.h"

int main(int argc, char **argv)
{
	// initialize SDL: video and sound
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// call SDL_Quit and SDL_CloseAudio exiting for a clean exit
	atexit(SDL_Quit);

    SDL_Surface* g_screen;
    g_screen = SDL_SetVideoMode(1024, 768, 0, 0);

    if (g_screen == NULL)
	{
		fprintf(stderr, "Unable to create screen: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}

    SDL_snd snd;

    snd.init(88200);

    const SndInfo* id;//  = snd.load_sound("t.wav");
    snd.play("t.wav", true);

	SDL_WM_SetCaption("Sound-Test", NULL);

	bool bDone = false;

    SDL_PauseAudio(0);

	while (!bDone)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				bDone = true;
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
                    bDone = true;
				} else snd.play("t.wav", true);
				break;

			case SDL_MOUSEMOTION:
				break;

			case SDL_MOUSEBUTTONDOWN:
				break;

			case SDL_MOUSEBUTTONUP:
                snd.play("title.wav");
                snd.unload(snd.load("t.wav"));
				break;

			}
		}

		SDL_Flip(g_screen);
	}

	return 0;
}

