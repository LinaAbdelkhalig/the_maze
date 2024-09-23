#include "../headers/headers.h"

uint8_t *MAP = NULL;
int MAP_SIZE = 0;

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
 * main - main function of the program
 * @argc: number of arguments passed
 * @argv: vector of arguments passed
 *
 * Return: 0
 */
int main(int argc, char *argv[])
{
	ASSERT((argc == 2), "Usage: %s <map_file_path>\n", argv[0]);
	/* load the map from the file */
	getMap(argv[1]);
	State state = {
		.running = true,
		.raining = 0,
		.miniMap = 0,
	};
	init(&state);
	load_sfx();
	init_textures(textureArray, &state);
	loadTextures(&state, &weaponShotTexture, "textures/gun_shot.png");
	loadTextures(&state, &weaponTexture, "textures/gun.png");
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
	SDL_SetRelativeMouseMode(true);
	Player player = {{4.0f, 4.0f}, {-1.0f, 0.0f}, {0.0f, 0.66f}};
	const float rotateSpeed = 0.2, moveSpeed = 0.30;
	bool isKilling = false;

	while (state.running)
	{
		const uint8_t *keystate = SDL_GetKeyboardState(NULL);
		int mouse_xrel = handle_events(&state, &isKilling);
		floatVector deltaPos;

		mouse_rotation(&mouse_xrel, rotateSpeed, &player);
		update_playerpos(&deltaPos, &player, keystate, moveSpeed);
		key_rotation_right(keystate, &player, &deltaPos, walkSFX,
				rotateSpeed, moveSpeed);
		key_rotation_left(keystate, &player, &deltaPos, walkSFX,
				rotateSpeed, moveSpeed);
		render_scene(&state, &player);
		renderWeapon(&state, isKilling);
		SDL_RenderPresent(state.renderer);
	}
	clean_up(&state, textureArray);
	return (0);
}
