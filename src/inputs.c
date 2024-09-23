#include "../headers/headers.h"

/**
 * handle_events - handles events and user input
 * @state: pointer to the state struct
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
						Mix_PlayChannel(1, rainSFX, -1);
					else
						Mix_HaltChannel(-1); }
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
 * @deltaPos: pointer to vector of change in player position
 * @player: pointer to the Player struct
 * @keystate: pointer to the var holding the key state
 * @moveSpeed: the movement speed
 *
 * Return: void
 */
void update_playerpos(floatVector *deltaPos, Player *player,
		const uint8_t *keystate, float moveSpeed)
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
