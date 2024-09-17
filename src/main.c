#include "../headers/headers.h"

/* #define MAP_SIZE 16
const uint8_t MAP[MAP_SIZE * MAP_SIZE] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2, 0, 1,
    1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2, 0, 1,
    1, 0, 0, 0, 0, 0, 3, 2, 2, 2, 2, 2, 2, 2, 0, 1,
    1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
*/
const float playerFOV = (PI / 2.0f);
const float maxDepth = 20.0f;

/* sound files */
Mix_Chunk *walkSFX = NULL;
Mix_Chunk *rainSFX = NULL;
Mix_Chunk *gunSFX = NULL;

SDL_Texture *textureArray[3];

void initializeTextures(SDL_Texture *textureArray[], State *state)
{
    for (int i = 0; i < 3; i++)
    {
        textureArray[i] = NULL;
    }

    loadTextures(state, &textureArray[0], "textures/floor_desert.png");
    loadTextures(state, &textureArray[1], "textures/wall_bricks_old_32.png");
    loadTextures(state, &textureArray[2], "textures/sky.png");
    // loadTextures(state, &textureArray[3], "textures/gun_shot.png");
    // loadTextures(state, &textureArray[4], "textures/gun.png");
}

/* textures */
// SDL_Texture *wallTexture = NULL;
// SDL_Texture *flrTexture = NULL;
// SDL_Texture *clngTexture = NULL;
SDL_Texture *weaponTexture = NULL;
SDL_Texture *weaponShotTexture = NULL;

int xy2index(int x, int y, int w)
{
	return (y * w + x);
}

void load_sfx()
{
	walkSFX = Mix_LoadWAV("sound/stepsnow_1.wav");
	ASSERT(walkSFX, "Could not load walkSFX. Error: %s\n",
			Mix_GetError());

	rainSFX = Mix_LoadWAV("sound/rain_2.wav");
	ASSERT(rainSFX, "Could not load rainSFX. Error: %s\n",
                        Mix_GetError());

	gunSFX = Mix_LoadWAV("sound/gunshot_23.wav");
	ASSERT(rainSFX, "Could not load gunSFX. Error: %s\n",
                        Mix_GetError());
}

void init()
{
	ASSERT(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO),
			"SDL failed to initialize; %s\n",
			SDL_GetError());

	ASSERT((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG),
			"Could not initialize SDL_image. Error: %s\n",
			IMG_GetError());


	/* initializing sdl_mixer for sfx */
	ASSERT(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0,
			"Could not initialize SDL_mixer. Error: %s\n",
			Mix_GetError());

}

void clean_up(State *state, SDL_Texture *textureArray[])
{
	Mix_FreeChunk(walkSFX);
	Mix_FreeChunk(rainSFX);
	Mix_FreeChunk(gunSFX);

	for(int i = 0; i < 3; i++)
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


int main(int argc, char *argv[])
{
	ASSERT((argc == 2), "Usage: %s <map_file_path>\n", argv[0]);
	/* load the map from the file */
	getMap(argv[1]);

	/* initialize sdl, sld_img, and sdl_mixer*/
	init();

	/* loading the sfx */
	load_sfx();

	State state = {
		.running = true,
		.raining = 0,
		.miniMap = 0,
	};

	/* create the window */
	state.window =
		SDL_CreateWindow("The Maze",
				SDL_WINDOWPOS_CENTERED_DISPLAY(0),
				SDL_WINDOWPOS_CENTERED_DISPLAY(0),
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN);
	ASSERT(state.window,
			"failed to create SDL window: %s\n",
			SDL_GetError());
	
	state.renderer = SDL_CreateRenderer(state.window, -1,
			SDL_RENDERER_PRESENTVSYNC);
	ASSERT(state.renderer,
			"failed to create SDL renderer: %s\n",
			SDL_GetError());

	/* set the textures to null and load them in the array*/
	initializeTextures(textureArray, &state);
	loadTextures(&state, &weaponShotTexture, "textures/gun_shot.png");
	loadTextures(&state, &weaponTexture, "textures/gun.png");

	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
	SDL_SetRelativeMouseMode(true);
	Player player = {
		.pos = {.x = 4.0f, .y = 4.0f},
		.dir = {.x = -1.0f, .y = 0.0f},
		.plane = {.x = 0.0f, .y = 0.66f},
	};
	const float rotateSpeed = 0.2, moveSpeed = 0.30;
	bool isKilling = false;

	/* the main loop */
	while (state.running)
	{
		SDL_Event event;
		int mouse_xrel = 0;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					state.running = false;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						state.running = false;
					if (event.key.keysym.sym == SDLK_SPACE)
					{
						isKilling = true;
						Mix_PlayChannel(-1, gunSFX, 0);
					}
					if (event.key.keysym.sym == SDLK_m)
						state.miniMap = !state.miniMap;
					if (event.key.keysym.sym == SDLK_r)
					{
						state.raining = !state.raining;

						/* play the rainSFX while it's raining */
						if(state.raining)
							Mix_PlayChannel(1, rainSFX, -1);
						else
							/* stop the rsin sfx */
							Mix_HaltChannel(-1);
					}
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_SPACE)
						isKilling = false;
					break;
				case SDL_MOUSEMOTION:
					mouse_xrel = event.motion.xrel;
					break;
				default:
					break;
			}
		}

		const uint8_t *keystate = SDL_GetKeyboardState(NULL);
		if (mouse_xrel != 0)
		{
			float rotSpeed = rotateSpeed * (mouse_xrel * -0.1);
			// both camera direction and camera plane must be rotated
			floatVector oldDir = player.dir;

			player.dir.x = player.dir.x * cosf(rotSpeed) - player.dir.y * sinf(rotSpeed);
			player.dir.y = oldDir.x * sinf(rotSpeed) + player.dir.y * cosf(rotSpeed);
			floatVector oldPlane = player.plane;
			player.plane.x = player.plane.x * cosf(rotSpeed) - player.plane.y * sinf(rotSpeed);
			player.plane.y = oldPlane.x * sinf(rotSpeed) + player.plane.y * cosf(rotSpeed);
		}
		
		floatVector deltaPos = {
			.x = player.dir.x * moveSpeed,
			.y = player.dir.y * moveSpeed,
		};
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP])
		{ /* move forward*/
			if (MAP[xy2index(player.pos.x + deltaPos.x,
						player.pos.y,
						MAP_SIZE)] == 0)
			{
				player.pos.x += deltaPos.x;
			}
			if (MAP[xy2index(player.pos.x, player.pos.y
						+ deltaPos.y,
						MAP_SIZE)] == 0)
			{
				player.pos.y += deltaPos.y;
			}
			Mix_PlayChannel(0, walkSFX, 0);
		}
		if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])
		{ /* backwards */
			if (MAP[xy2index(player.pos.x - deltaPos.x,
						player.pos.y,
						MAP_SIZE)] == 0)
			{
				player.pos.x -= deltaPos.x;
			}
			if (MAP[xy2index(player.pos.x,
						player.pos.y - deltaPos.y,
						MAP_SIZE)] == 0)
			{
				player.pos.y -= deltaPos.y;
			}
			Mix_PlayChannel(0, walkSFX, 0);
		}
		if (keystate[SDL_SCANCODE_D] ||
				keystate[SDL_SCANCODE_RIGHT])
		{ // turn right
			floatVector oldDir = player.dir;
			player.dir.x = oldDir.x * cosf(-rotateSpeed) - oldDir.y * sinf(-rotateSpeed);
			player.dir.y = oldDir.x * sinf(-rotateSpeed) + oldDir.y * cosf(-rotateSpeed);
			floatVector oldPlane = player.plane;
			player.plane.x = oldPlane.x * cosf(-rotateSpeed) - oldPlane.y * sinf(-rotateSpeed);
			player.plane.y = oldPlane.x * sinf(-rotateSpeed) + oldPlane.y * cosf(-rotateSpeed);
			deltaPos.x = player.plane.x * moveSpeed;
			deltaPos.y = player.plane.y * moveSpeed;
			if (MAP[xy2index(player.pos.x + deltaPos.x,
						player.pos.y,
						MAP_SIZE)] == 0)
			{
				player.pos.x += deltaPos.x;
			}
			if (MAP[xy2index(player.pos.x,
						player.pos.y + deltaPos.y,
						MAP_SIZE)] == 0)
			{
				player.pos.y += deltaPos.y;
			}
			Mix_PlayChannel(0, walkSFX, 0);
		}
		if (keystate[SDL_SCANCODE_A] ||
				keystate[SDL_SCANCODE_LEFT])
		{ // turn left
			floatVector oldDir = player.dir;
			player.dir.x = oldDir.x * cosf(rotateSpeed) - oldDir.y * sinf(rotateSpeed);
			player.dir.y = oldDir.x * sinf(rotateSpeed) + oldDir.y * cosf(rotateSpeed);
			floatVector oldPlane = player.plane;
			player.plane.x = oldPlane.x * cosf(rotateSpeed) - oldPlane.y * sinf(rotateSpeed);
			player.plane.y = oldPlane.x * sinf(rotateSpeed) + oldPlane.y * cosf(rotateSpeed);
			deltaPos.x = player.plane.x * moveSpeed;
			deltaPos.y = player.plane.y * moveSpeed;
			if (MAP[xy2index(player.pos.x - deltaPos.x,
						player.pos.y,
						MAP_SIZE)] == 0)
			{
				player.pos.x -= deltaPos.x;
			}
			if (MAP[xy2index(player.pos.x,
						player.pos.y - deltaPos.y,
						MAP_SIZE)] == 0)
			{
				player.pos.y -= deltaPos.y;
			}
			Mix_PlayChannel(0, walkSFX, 0);
		}
		SDL_RenderClear(state.renderer);
		renderFloor(&state, &player, 64, 64, &textureArray[0]);  // Floor texture
		renderCeiling(&state, &player, 64, 64, &textureArray[2]);  // Ceiling texture
		renderWall(&state, &player, 32, 32, &textureArray[1]);  // Wall texture

		if (state.miniMap)
			renderMiniMap(&state, &player, 150);
		if (state.raining)
		{
			renderRain(&state);
		}
		renderWeapon(&state, isKilling);
		SDL_RenderPresent(state.renderer);
	}
	SDL_DestroyTexture(weaponTexture);
	SDL_DestroyTexture(weaponShotTexture);
	

	clean_up(&state, textureArray);

	return (0);
}
