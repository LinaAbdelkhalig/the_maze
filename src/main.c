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

int xy2index(int x, int y, int w)
{
	return (y * w + x);
}

SDL_Texture *wallTexture = NULL;
SDL_Texture *flrTexture = NULL;
SDL_Texture *clngTexture = NULL;

int texWidth = 16;
int texHeight = 16;

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

	State state = {
		.running = true,
		.raining = 0,
	};
	state.window =
		SDL_CreateWindow("Raycast",
				SDL_WINDOWPOS_CENTERED_DISPLAY(0),
				SDL_WINDOWPOS_CENTERED_DISPLAY(0),
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				SDL_WINDOW_ALLOW_HIGHDPI);
	ASSERT(state.window,
			"failed to create SDL window: %s\n",
			SDL_GetError());
	state.renderer = SDL_CreateRenderer(state.window, -1,
			SDL_RENDERER_PRESENTVSYNC);
	ASSERT(state.renderer,
			"failed to create SDL renderer: %s\n",
			SDL_GetError());
	loadTextures(&state, &flrTexture, "textures/bronze_plank.png");
	loadTextures(&state, &wallTexture, "textures/wall_bricks_old_32.png");
	loadTextures(&state, &clngTexture, "textures/bronze_plank.png");
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
	SDL_SetRelativeMouseMode(true);
	Player player = {
		.pos = {.x = 4.0f, .y = 4.0f},
		.dir = {.x = -1.0f, .y = 0.0f},
		.plane = {.x = 0.0f, .y = 0.66f},
	};
	const float rotateSpeed = 0.025, moveSpeed = 0.05;

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
				case SDL_MOUSEMOTION:
					mouse_xrel = event.motion.xrel;
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
		{ // backwards
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
		renderFlrClng(&state, &player, texWidth, texHeight, &flrTexture, &clngTexture);
		renderWall(&state, &player, texWidth, texHeight, &wallTexture);
		if (state.raining)
		{
			renderRain(&state);
		}
		SDL_RenderPresent(state.renderer);
	}
	SDL_DestroyTexture(wallTexture);
	SDL_DestroyRenderer(state.renderer);
	SDL_DestroyWindow(state.window);
	IMG_Quit();
	SDL_Quit();
	return (0);
}
