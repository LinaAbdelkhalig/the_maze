#include "../headers/headers.h"

/**
 * renderWeapon - Draw the weapon.
 *
 * @state: The state of the game.
 * @isKilling: Is the player's weapon used or idle.
 */
void renderWeapon(State *state, bool isKilling)
{
	int weaponW = SCREEN_WIDTH / 2;
	int weaponH = SCREEN_HEIGHT / 2;

	SDL_Rect weaponRect = {
		.x = (int)(SCREEN_WIDTH - weaponW),
		.y = (int)(SCREEN_HEIGHT - weaponH),
		.w = weaponW,
		.h = weaponH
	};
	if (isKilling)
		SDL_RenderCopyEx(state->renderer, weaponShotTexture,
				NULL, &weaponRect, 0.0f, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(state->renderer, weaponTexture,
				NULL, &weaponRect, 0.0f, NULL, SDL_FLIP_NONE);
}
