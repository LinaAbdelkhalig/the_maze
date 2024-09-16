#include "../headers/headers.h"

/**
 * isWall - Checks if there is a wall in the specified coordinates.
 *
 * @x: The x-coordinate.
 * @y: The y-coordinate.
 *
 * Return: true if there is a wall, false if not.
 */
bool isWall(float x, float y)
{
	if (x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE)
		return MAP[xy2index((int)x, (int)y, MAP_SIZE)] >= 1;
	return false;
}

/**
 * renderMiniMap - Render the map on a mini-map in the left-top of the window.
 *
 * @state: The state of the game.
 * @player: The player.
 * @miniMapSize: The size of the mini map to be drawn.
 */
void renderMiniMap(State *state, Player *player, int miniMapSize)
{
	/* Define mini-map parameters */
	SDL_Rect miniMapRect = {0, 0, miniMapSize, miniMapSize};
	SDL_Rect tileRect = {0, 0, miniMapSize / MAP_SIZE, miniMapSize / MAP_SIZE};

	/* Clear the mini-map area */
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(state->renderer, &miniMapRect);

	/* Render the map on the mini-map */
	for (int y = 0; y < MAP_SIZE; y++)
	{
		for(int x = 0; x < MAP_SIZE; x++)
		{
			uint8_t tile = MAP[xy2index(x, y, MAP_SIZE)];
			if (tile == 0) /* empty, set color to brown */
				SDL_SetRenderDrawColor(state->renderer, 165, 42, 42, 255);
			else /* wall, set color to gray */
				SDL_SetRenderDrawColor(state->renderer, 169, 169, 169, 255);
			tileRect.x = x * tileRect.w;
			tileRect.y = y * tileRect.h;
			SDL_RenderFillRect(state->renderer, &tileRect);
		}
	}

	/*Render the player position on the mini-map*/
	SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255); /*white player*/
	SDL_Rect playerPosRect = {
		.x = (player->pos.x / MAP_SIZE) * miniMapSize,
		.y = (player->pos.y / MAP_SIZE) * miniMapSize,
		.w = miniMapSize / MAP_SIZE,
		.h = miniMapSize / MAP_SIZE
	};
	SDL_RenderFillRect(state->renderer, &playerPosRect);

	/*Render the player's line of sight*/
	float rayDirX = player->dir.x, rayDirY = player->dir.y;
	float startX = player->pos.x, startY = player->pos.y;
	float stepSize = 0.1f; /*smaller steps*/
	float maxDistance = MAP_SIZE;/*max distance to check*/
	float endX = startX, endY = startY;
	/* Make the line stop when there is a wall */
	for (float dist = 0; dist< maxDistance; dist +=stepSize)
	{
		endX = startX + rayDirX * dist;
		endY = startY + rayDirY * dist;
		if (isWall(endX, endY))
			break;
	}
	SDL_SetRenderDrawColor(state->renderer, 255, 255, 0, 100); /*transpernt yellow*/
	int sightEndX = (endX/MAP_SIZE) * miniMapSize;
	int sightEndY = (endY/MAP_SIZE) * miniMapSize;
	SDL_RenderDrawLine(state->renderer,
			playerPosRect.x + playerPosRect.w / 2,
			playerPosRect.y + playerPosRect.h / 2,
			sightEndX,
			sightEndY);

	/*Reset the color to black*/
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
}
