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

int xy2index(int x, int y, int w)
{
	return (y * w + x);
}

void load_sfx()
{
	walkSFX = Mix_LoadWAV("sound/stepdirt_2.wav");
	ASSERT(walkSFX, "Could not load walkSFX. Error: %s\n",
			Mix_GetError());

	rainSFX = Mix_LoadWAV("sound/rain_2.wav");
	ASSERT(rainSFX, "Could not load rainSFX. Error: %s\n",
                        Mix_GetError());
}

void clean_up(State *state)
{
	Mix_FreeChunk(walkSFX);
	Mix_FreeChunk(rainSFX);
	SDL_DestroyRenderer(state->renderer);
	SDL_DestroyWindow(state->window);
	Mix_CloseAudio();

	Mix_Quit();
	SDL_Quit();
}

SDL_Texture *wallTexture = NULL;
SDL_Texture *flrTexture = NULL;
SDL_Texture *clngTexture = NULL;
SDL_Texture *weaponTexture = NULL;
SDL_Texture *weaponShotTexture = NULL;

int main(int argc, char *argv[])
{
	ASSERT((argc == 2), "Usage: %s <map_file_path>\n", argv[0]);
	/* load the map from the file */
	getMap(argv[1]);
	ASSERT(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO),
			"SDL failed to initialize; %s\n",
			SDL_GetError());

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("Could not initialize SDL_image. Error:%s\n", IMG_GetError());
		return (1);
	}

	/* initializing sdl_mixer for sfx */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("could not initialize SDL_mixer. Error: %s\n",
				Mix_GetError());
	}

	/* loading the sfx */
	load_sfx();

	State state = {
		.running = true,
		.raining = 0,
	};
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
	loadTextures(&state, &flrTexture, "textures/floor_desert.png");
	loadTextures(&state, &wallTexture, "textures/wall_bricks_old_32.png");
	loadTextures(&state, &clngTexture, "textures/sky.png");
	loadTextures(&state, &weaponShotTexture, "textures/gun_shot.png");
	loadTextures(&state, &weaponTexture, "textures/gun.png");
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
	SDL_SetRelativeMouseMode(true);
	Player player = {
		.pos = {.x = 4.0f, .y = 4.0f},
		.dir = {.x = -1.0f, .y = 0.0f},
		.plane = {.x = 0.0f, .y = 0.66f},
	};
	const float rotateSpeed = 0.1, moveSpeed = 0.15;
	bool isKilling = false;
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
					if (event.key.keysym.sym == SDLK_SPACE)
						isKilling = true;
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
		if (keystate[SDL_SCANCODE_ESCAPE])
			state.running = false;
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
		/* r to stop/start rain */
		if (keystate[SDL_SCANCODE_R])
		{
			state.raining = !state.raining;

			/* play the rainSFX while it's raining */
			if(state.raining)
			{
				Mix_PlayChannel(-1, rainSFX, -1);
			}
			else
			{
				/* stop the rsin sfx */
				Mix_HaltChannel(-1);
			}
		}
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP])
		{ 
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
		}
		SDL_RenderClear(state.renderer);
		renderFloor(&state, &player, 64, 64, &flrTexture);
		renderCeiling(&state, &player, 64, 64, &clngTexture);
		renderWall(&state, &player, 32, 32, &wallTexture);
		if (state.raining)
		{
			renderRain(&state);
		}
		renderWeapon(&state, isKilling);
		SDL_RenderPresent(state.renderer);
	}
	SDL_DestroyTexture(weaponTexture);
	SDL_DestroyTexture(weaponShotTexture);
	SDL_DestroyTexture(flrTexture);
	SDL_DestroyTexture(clngTexture);
	SDL_DestroyTexture(wallTexture);
	SDL_DestroyRenderer(state.renderer);
	SDL_DestroyWindow(state.window);
	IMG_Quit();
	SDL_Quit();
	return (0);
}
