#include "../headers/headers.h"
/**
 * renderCeiling - renders the ceiling texture
 * @state: The struct holding the window and renderer
 * @player: The struct holding the players position information
 * @texWidth: Width of the texture
 * @texHeight: The height of the textures
 * @clngTexture: The var holding the texture for the ceiling
 */

void renderCeiling(State *state, Player *player, int texWidth, int texHeight, SDL_Texture **clngTexture)
{
	double rayDirX0 = player->dir.x - player->plane.x;
	double rayDirY0 = player->dir.y - player->plane.y;
	double rayDirX1 = player->dir.x + player->plane.x;
	double rayDirY1 = player->dir.y + player->plane.y;
	double posZ = 0.5 * SCREEN_HEIGHT;

	// Pre-create SDL_Rect objects
	SDL_Rect srcRect = {0, 0, 1, 1};
	SDL_Rect dstRectCeiling = {0, 0, 1, 1};

	// Loop through rows (upper half of the screen)
	for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; ++y)
	{
		int p = y - SCREEN_HEIGHT / 2;
		double rowDistance = posZ / p;

		// Precompute steps for this row
		double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

		// Initial floor positions
		double floorX = player->pos.x + rowDistance * rayDirX0;
		double floorY = player->pos.y + rowDistance * rayDirY0;

		dstRectCeiling.y = SCREEN_HEIGHT - y - 1;  // Adjust for ceiling

		// Loop through each column (pixels on the row)
		for (int x = 0; x < SCREEN_WIDTH; ++x)
		{
			// Calculate texture coordinates
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);
			int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
			int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

			// Set texture source and destination positions
			srcRect.x = tx;
			srcRect.y = ty;
			dstRectCeiling.x = x;

			// Render ceiling pixel
			SDL_RenderCopy(state->renderer, *clngTexture, &srcRect, &dstRectCeiling);

			// Update floor position for the next pixel
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
 * @clngTexture: The var holding the texture for the ceiling
 */

void renderFloor(State *state, Player *player, int texWidth,
		int texHeight, SDL_Texture **flrTexture)
{
	double rayDirX0 = player->dir.x - player->plane.x;
	double rayDirY0 = player->dir.y - player->plane.y;
	double rayDirX1 = player->dir.x + player->plane.x;
	double rayDirY1 = player->dir.y + player->plane.y;
	double posZ = 0.5 * SCREEN_HEIGHT;

	// Create rectangles outside the loop
	SDL_Rect srcRect = {0, 0, 1, 1};
	SDL_Rect dstRectFloor = {0, 0, 1, 1};

	// Loop through the screen rows (only lower half)
	for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; ++y)
	{
		int p = y - SCREEN_HEIGHT / 2;
		double rowDistance = posZ / p;

		// Precompute steps for this row
		double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

		// Initial floor positions
		double floorX = player->pos.x + rowDistance * rayDirX0;
		double floorY = player->pos.y + rowDistance * rayDirY0;

		dstRectFloor.y = y;  // Set row for destination

		// Loop through each column (pixels on the row)
		for (int x = 0; x < SCREEN_WIDTH; ++x)
		{
			// Calculate texture coordinates
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);
			int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
			int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

			// Set texture source and destination positions
			srcRect.x = tx;
			srcRect.y = ty;
			dstRectFloor.x = x;

			// Render floor pixel
			SDL_RenderCopy(state->renderer, *flrTexture, &srcRect, &dstRectFloor);

			// Update floor position for next pixel
			floorX += floorStepX;
			floorY += floorStepY;
		}
	}
}


void renderWall(State *state, Player *player, int texWidth, int texHeight, SDL_Texture **wallTexture)
{
	for (int x = 0; x < SCREEN_WIDTH; ++x)
	{
		float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
		floatVector rayDir = {
			.x = player->dir.x + player->plane.x * cameraX,
			.y = player->dir.y + player->plane.y * cameraX,
		};
		IntVector mapBox = {
			.x = (int)player->pos.x,
			.y = (int)player->pos.y};
		floatVector sideDist = {0.0, 0.0};
		floatVector deltaDist = {
			.x = (rayDir.x == 0) ? 1e30 : fabsf(1 / rayDir.x),
			.y = (rayDir.y == 0) ? 1e30 : fabsf(1 / rayDir.y),
		};
		float perpWallDist;
		IntVector stepDir = {0, 0};
		bool hit = false;
		Side side;
		if (rayDir.x < 0)
		{
			stepDir.x = -1;
			sideDist.x = (player->pos.x - mapBox.x) * deltaDist.x;
		}
		else
		{
			stepDir.x = 1;
			sideDist.x = (mapBox.x + 1.0f - player->pos.x) * deltaDist.x;
		}
		if (rayDir.y < 0)
		{
			stepDir.y = -1;
			sideDist.y = (player->pos.y - mapBox.y) * deltaDist.y;
		}
		else
		{
			stepDir.y = 1;
			sideDist.y = (mapBox.y + 1.0f - player->pos.y) * deltaDist.y;
		}
		while (!hit)
		{
			if (sideDist.x < sideDist.y)
			{
				sideDist.x += deltaDist.x;
				mapBox.x += stepDir.x;
				side = EastWest;
			}
			else
			{
				sideDist.y += deltaDist.y;
				mapBox.y += stepDir.y;
				side = NorthSouth;
			}
			if (MAP[xy2index(mapBox.x, mapBox.y, MAP_SIZE)] > 0)
			{
				hit = true;
			}
		}
		if (side == EastWest)
		{
			perpWallDist = (sideDist.x - deltaDist.x);
		}
		else
		{
			perpWallDist = (sideDist.y - deltaDist.y);
		}
		int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
		int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawEnd >= SCREEN_HEIGHT)
			drawEnd = SCREEN_HEIGHT;
		float wallX;
		if (side == EastWest)
		{
			wallX = player->pos.y + perpWallDist * rayDir.y;
		}
		else
		{
			wallX = player->pos.x + perpWallDist * rayDir.x;
		}
		wallX -= floorf(wallX);
		int texX = (int)(wallX * (float)texWidth);
		if (side == EastWest && rayDir.x > 0)
			texX = texWidth - texX - 1;
		if (side == NorthSouth && rayDir.y < 0)
			texX = texWidth - texX - 1;
		Uint8 r = 255, g = 255, b = 255;
		if (side == NorthSouth)
		{
			r = 160;
			g = 160;
			b = 160;
		}
		SDL_SetTextureColorMod(*wallTexture, r, g, b);
		SDL_Rect srcRect = {texX, 0, 1, texHeight};
		SDL_Rect destRect = {x, drawStart, 1, lineHeight};
		SDL_RenderCopy(state->renderer, *wallTexture, &srcRect, &destRect);
	}
}
