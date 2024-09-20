#include "../headers/headers.h"

const float playerFOV = (PI / 2.0f);
const float maxDepth = 20.0f;

/* sound files */
Mix_Chunk *walkSFX = NULL;
Mix_Chunk *rainSFX = NULL;
Mix_Chunk *gunSFX = NULL;

SDL_Texture *textureArray[3];
/*TODO: add these to the initTextures function*/
SDL_Texture *weaponTexture = NULL;
SDL_Texture *weaponShotTexture = NULL;

/**
 * init_textures - initializes and loads the textures from PNGs
 * @textureArray: array that holds the textures to be loaded
 * @state: struct holding renderer and window
 * Return: void
 */
void init_textures(SDL_Texture *textureArray[], State *state)
{
	for (int i = 0; i < 3; i++)
	{
		textureArray[i] = NULL;
	}
	loadTextures(state, &textureArray[0], "textures/floor_desert.png");
	loadTextures(state, &textureArray[1], "textures/wall_bricks_old_32.png");
	loadTextures(state, &textureArray[2], "textures/sky.png");
	/* TODO: add textures for the gun and gunshots */
}

/**
 * xy2index - converts x and y indices to array index
 * @x: horizontal coordinate
 * @y: vertical coordinate
 * @w: map size/width
 *
 * Return: array index
 */
int xy2index(int x, int y, int w)
{
	return (y * w + x);
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

/** handle_events - handles events and user input
 * @state: pointer to the state struct
 * @player: pointer to the player struct
 * @isKilling: determines whether the weapon is shooting
 *
 * Return: void
 */
int handle_events(State *state, bool *isKilling)
{
	SDL_Event event;
	int mouse_xrel = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				state->running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					state->running = false;
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					*isKilling = true;
					Mix_PlayChannel(-1, gunSFX, 0);
				}
				if (event.key.keysym.sym == SDLK_m)
					state->miniMap = !state->miniMap;
				if (event.key.keysym.sym == SDLK_r)
				{
					state->raining = !state->raining;
					if (state->raining)
						Mix_PlayChannel(1, rainSFX,
							       	-1);
					else
						Mix_HaltChannel(-1);
				}
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_SPACE)
					*isKilling = false;
				break;
			case SDL_MOUSEMOTION:
				mouse_xrel = event.motion.xrel;
				break;
			default:
				break;
		}
	}
	return (mouse_xrel);
}
/**
 * update_playerpos - updates the position of the player
 * @player: pointer to the Player struct
 * @keystate: pointer to the var holding the key state
 * @moveSpeed: the movement speed
 * @rotateSpeed: the rotation speed
 *
 * Return: void
 */
void update_playerpos(floatVector *deltaPos, Player *player, const uint8_t *keystate, float moveSpeed)
{
	deltaPos->x = player->dir.x * moveSpeed;
	deltaPos->y = player->dir.y * moveSpeed;

	/* move forward with W or up key */
	if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP])
	{
		if (MAP[xy2index(player->pos.x + deltaPos->x, player->pos.y,
					MAP_SIZE)] == 0)
			player->pos.x += deltaPos->x;
		if (MAP[xy2index(player->pos.x, player->pos.y + deltaPos->y,
					MAP_SIZE)] == 0)
			player->pos.y += deltaPos->y;
		Mix_PlayChannel(0, walkSFX, 0);
	}
	/* move backwards with S or down key */
	if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])
	{
		if (MAP[xy2index(player->pos.x - deltaPos->x, player->pos.y,
					MAP_SIZE)] == 0)
			player->pos.x -= deltaPos->x;
		if (MAP[xy2index(player->pos.x, player->pos.y - deltaPos->y,
					MAP_SIZE)] == 0)
			player->pos.y -= deltaPos->y;
		Mix_PlayChannel(0, walkSFX, 0);
	}
}

/**
 * render_scene - redners the walls, ceiling and floor
 * @state: pointer to the state struct
 * @player: pointer to the player struct
 * 
 * Return: void
 */
void render_scene(State *state, Player *player)
{
	SDL_RenderClear(state->renderer);
	renderFloor(state, player, 64, 64, &textureArray[0]);
	renderCeiling(state, player, 64, 64, &textureArray[2]);
	renderWall(state, player, 32, 32, &textureArray[1]);
	if (state->miniMap)
		renderMiniMap(state, player, 150);
	if (state->raining)
		renderRain(state);
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

	for (int i = 0; i < 3; i++)
	{
		SDL_DestroyTexture(textureArray[i]);
	}
	SDL_DestroyTexture(weaponTexture);
	SDL_DestroyTexture(weaponShotTexture);
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

	/* assign the initial values of state */
	State state = {
		.running = true,
		.raining = 0,
		.miniMap = 0,
	};
	/* initialize sdl, sld_img, and sdl_mixer, and create the window*/
	init(&state);

	/* loading the sfx */
	load_sfx();

	/* set the textures to null and load them in the array*/
	init_textures(textureArray, &state);
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
		const uint8_t *keystate = SDL_GetKeyboardState(NULL);
		int mouse_xrel = handle_events(&state, &isKilling);
		
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

		floatVector deltaPos;
		update_playerpos(&deltaPos, &player, keystate, moveSpeed);
		
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

		render_scene(&state, &player);
		renderWeapon(&state, isKilling);
		SDL_RenderPresent(state.renderer);
	}
	
	clean_up(&state, textureArray);

	return (0);
}
