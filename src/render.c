#include "../headers/headers.h"

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
 * renderCeiling - renders the ceiling texture
 *
 * @state: The struct holding the window and renderer
 * @player: The struct holding the players position information
 * @texWidth: Width of the texture
 * @texHeight: The height of the textures
 * @clngTexture: The var holding the texture for the ceiling
 */
void renderCeiling(State *state, Player *player, int texWidth,
		int texHeight, SDL_Texture **clngTexture)
{
	double rayDirX0 = player->dir.x - player->plane.x;
	double rayDirY0 = player->dir.y - player->plane.y;
	double rayDirX1 = player->dir.x + player->plane.x;
	double rayDirY1 = player->dir.y + player->plane.y;
	double posZ = 0.5 * SCREEN_HEIGHT;

	/* Pre-create SDL_Rect objects */
	SDL_Rect srcRect = {0, 0, 1, 1};
	SDL_Rect dstRectCeiling = {0, 0, 1, 1};
	/* Loop through rows (upper half of the screen) */
	for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; ++y)
	{
		int p = y - SCREEN_HEIGHT / 2;
		double rowDistance = posZ / p;
		/* Precompute steps for this row */
		double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;
		/* Initial floor positions */
		double floorX = player->pos.x + rowDistance * rayDirX0;
		double floorY = player->pos.y + rowDistance * rayDirY0;

		dstRectCeiling.y = SCREEN_HEIGHT - y - 1;/*Adjust for ceiling*/
		/* Loop through each column (pixels on the row) */
		for (int x = 0; x < SCREEN_WIDTH; ++x)
		{ /* Calculate texture coordinates */
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);
			int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
			int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);
			/* Set texture source and destination positions */
			srcRect.x = tx;
			srcRect.y = ty;
			dstRectCeiling.x = x;
			/* Render ceiling pixel */
			SDL_RenderCopy(state->renderer, *clngTexture, &srcRect, &dstRectCeiling);
			/* Update floor position for the next pixel */
			floorX += floorStepX;
			floorY += floorStepY;
		}
	}
}


/**
 * renderFlrClng - Renders the textures for the ceiling and floor
 *
 * @state: The struct holding the window and renderer
 * @player: The struct holding the players position information
 * @texWidth: Width of the texture
 * @texHeight: The height of the textures
 * @flrTexture: The var holding the texture for the floor
 */
void renderFloor(State *state, Player *player, int texWidth,
		int texHeight, SDL_Texture **flrTexture)
{
	double rayDirX0 = player->dir.x - player->plane.x;
	double rayDirY0 = player->dir.y - player->plane.y;
	double rayDirX1 = player->dir.x + player->plane.x;
	double rayDirY1 = player->dir.y + player->plane.y;
	double posZ = 0.5 * SCREEN_HEIGHT;
	/* Create rectangles outside the loop */
	SDL_Rect srcRect = {0, 0, 1, 1};
	SDL_Rect dstRectFloor = {0, 0, 1, 1};
	/* Loop through the screen rows (only lower half) */
	for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; ++y)
	{
		int p = y - SCREEN_HEIGHT / 2;
		double rowDistance = posZ / p;
		/* Precompute steps for this row */
		double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;
		/* Initial floor positions */
		double floorX = player->pos.x + rowDistance * rayDirX0;
		double floorY = player->pos.y + rowDistance * rayDirY0;

		dstRectFloor.y = y;  /* Set row for destination */
		/* Loop through each column (pixels on the row) */
		for (int x = 0; x < SCREEN_WIDTH; ++x)
		{
			/* Calculate texture coordinates */
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);
			int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
			int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);
			/* Set texture source and destination positions */
			srcRect.x = tx;
			srcRect.y = ty;
			dstRectFloor.x = x;
			/* Render floor pixel */
			SDL_RenderCopy(state->renderer, *flrTexture, &srcRect, &dstRectFloor);
			/* Update floor position for next pixel */
			floorX += floorStepX;
			floorY += floorStepY;
		}
	}
}


/**
 * renderWall - render the walls using raycasting
 * @state: pointer to the game state (State struct)
 * @player: pointer to the Player struct
 * @texWidth: texture width
 * @texHeight: texture height
 * @wallTexture: pointer to SDL_Texture for the wall
 * Return: Void
 */
void renderWall(State *state, Player *player, int texWidth, int texHeight,
		SDL_Texture **wallTexture)
{
	for (int x = 0; x < SCREEN_WIDTH; ++x)
	{
		float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
		floatVector rayDir = calculate_raydir(player, cameraX);
		IntVector mapBox = { (int)player->pos.x, (int)player->pos.y };
		floatVector deltaDist = { fabsf(1 / rayDir.x), fabsf(1 / rayDir.y) };
		floatVector sideDist = { 0, 0 };
		IntVector stepDir = { 0, 0 };
		Side side;

		step_sidedist(player, rayDir, &mapBox, &deltaDist, &stepDir,
				&sideDist);
		while (!perform_dda(&mapBox, &stepDir, &sideDist,
					&deltaDist, &side))
		{}
		float perpWallDist = (side == EastWest) ?
			(sideDist.x - deltaDist.x) :
			(sideDist.y - deltaDist.y);
		float wallX = calculate_wallx(player, rayDir, perpWallDist,
				side) - floorf(calculate_wallx(player, rayDir,
						perpWallDist, side));
		int texX = (int)(wallX * (float)texWidth);

		if ((side == EastWest && rayDir.x > 0) || (side == NorthSouth
					&& rayDir.y < 0))
			texX = texWidth - texX - 1;
		int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
		int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;

		Uint8 r = (side == NorthSouth) ? 160 : 255, g = r, b = r;

		SDL_SetTextureColorMod(*wallTexture, r, g, b);
		SDL_Rect srcRect = { texX, 0, 1, texHeight };
		SDL_Rect destRect = { x, drawStart, 1, lineHeight };

		SDL_RenderCopy(state->renderer, *wallTexture, &srcRect,
				&destRect);
	}
}
