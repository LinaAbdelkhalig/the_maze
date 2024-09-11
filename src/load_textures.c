#include "../headers/headers.h"

void loadTextures(State *state, SDL_Texture **texture, const char *path) 
{
    SDL_Surface* surface = IMG_Load(path);
    ASSERT(surface, "Failed to load texture: %s\n", IMG_GetError());
    *texture = SDL_CreateTextureFromSurface(state->renderer, surface);
    SDL_FreeSurface(surface);
    ASSERT(*texture, "Failed to create texture: %s\n", SDL_GetError());
}
