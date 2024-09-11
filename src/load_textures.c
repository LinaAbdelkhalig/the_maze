#include "../headers/headers.h"

/**
 * loadTextures - Creates a texture from a surface.
 *
 * @state: The variable containing the window and renderer
 * @texture: The variable that the texture will be loaded into
 * @path: the path of the image from which the texture will be creatd
 */
void loadTextures(State *state, SDL_Texture **texture, const char *path)
{
	SDL_Surface *surface = IMG_Load(path);

	ASSERT(surface, "Failed to load texture: %s\n", IMG_GetError());
	*texture = SDL_CreateTextureFromSurface(state->renderer, surface);
	SDL_FreeSurface(surface);
	ASSERT(*texture, "Failed to create texture: %s\n", SDL_GetError());
}
