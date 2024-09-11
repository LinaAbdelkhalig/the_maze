#include "../headers/headers.h"

/**
 * loadTextures - Loads texture to the walls.
 *
 * @state: The state of the game.
 * @wallTexture: The wall texture.
 */
void loadTextures(State *state, SDL_Texture **wallTexture)
{
	SDL_Surface *surface = IMG_Load("textures/wall_bricks_old_1024.png");

	ASSERT(surface, "Failed to load texture: %s\n", IMG_GetError());
	*wallTexture = SDL_CreateTextureFromSurface(state->renderer, surface);
	SDL_FreeSurface(surface);
	ASSERT(*wallTexture, "Failed to create texture: %s\n", SDL_GetError());
}
