#include "../headers/headers.h"

/* Definitions for global variables */
Mix_Chunk *walkSFX = NULL;
Mix_Chunk *rainSFX = NULL;
Mix_Chunk *gunSFX = NULL;
SDL_Texture *textureArray[5];

/**
 * init - initializes SDL, IMG, and SDL_Mixer
 * @state: pointer to the struct that holds window and render information
 *
 * Return: void
 */
void init(State *state)
{
	ASSERT(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO),
			"SDL failed to initialize; %s\n", SDL_GetError());

	ASSERT((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG),
			"Could not initialize SDL_image. Error: %s\n",
			IMG_GetError());

	/* initializing sdl_mixer for sfx */
	ASSERT(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0,
			"Could not initialize SDL_mixer. Error: %s\n",
			Mix_GetError());

	/* create the window */
	state->window = SDL_CreateWindow("The Maze",
			SDL_WINDOWPOS_CENTERED_DISPLAY(0),
			SDL_WINDOWPOS_CENTERED_DISPLAY(0),
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
	ASSERT(state->window, "failed to create SDL window: %s\n", SDL_GetError());

	/* Create the renderer */
	state->renderer = SDL_CreateRenderer(state->window, -1,
			SDL_RENDERER_PRESENTVSYNC);
	ASSERT(state->renderer, "failed to create SDL renderer: %s\n",
			SDL_GetError());
}

/**
 * init_textures - initializes and loads the textures from PNGs
 * @textureArray: array that holds the textures to be loaded
 * @state: struct holding renderer and window
 *
 * Return: void
 */
void init_textures(SDL_Texture *textureArray[], State *state)
{
	for (int i = 0; i < 5; i++)
	{
		textureArray[i] = NULL;
	}
	loadTextures(state, &textureArray[0], "textures/floor_desert.png");
	loadTextures(state, &textureArray[1], "textures/wall_bricks_old_32.png");
	loadTextures(state, &textureArray[2], "textures/sky.png");
	loadTextures(state, &textureArray[3], "textures/gun.png");
	loadTextures(state, &textureArray[4], "textures/gun_shot.png");
}

/**
 * load_sfx - loads the sound effects from the wav files
 *
 * Return: void
 */
void load_sfx(void)
{
	walkSFX = Mix_LoadWAV("sound/stepsnow_1.wav");
	ASSERT(walkSFX, "Could not load walkSFX. Error: %s\n", Mix_GetError());

	rainSFX = Mix_LoadWAV("sound/rain_2.wav");
	ASSERT(rainSFX, "Could not load rainSFX. Error: %s\n", Mix_GetError());

	gunSFX = Mix_LoadWAV("sound/gunshot_23.wav");
	ASSERT(rainSFX, "Could not load gunSFX. Error: %s\n", Mix_GetError());
}

/**
 * clean_up - frees resources and quits SDL
 * @state: pointer to the state struct
 * @textureArray: array that holds the textures
 *
 * Return: void
 */
void clean_up(State *state, SDL_Texture *textureArray[])
{
	Mix_FreeChunk(walkSFX);
	Mix_FreeChunk(rainSFX);
	Mix_FreeChunk(gunSFX);

	for (int i = 0; i < 5; i++)
	{
		SDL_DestroyTexture(textureArray[i]);
	}
	SDL_DestroyRenderer(state->renderer);
	SDL_DestroyWindow(state->window);
	Mix_CloseAudio();

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}
