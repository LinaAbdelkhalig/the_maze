#include"../headers/headers.h"

/**
 * calc_raydir - Function to calculate ray direction
 * @player: pointer to the Player struct
 * @cameraX: the position of the camera on the X-axis
 *
 * Return: floatVector representing the direction of the ray
 */
floatVector calculate_raydir(Player *player, float cameraX)
{
	floatVector rayDir;

	rayDir.x = player->dir.x + player->plane.x * cameraX;
	rayDir.y = player->dir.y + player->plane.y * cameraX;
	return (rayDir);
}

/**
 * step_sidestep - calculate the side distances and step directions
 * @player: pointer to the Player struct
 * @rayDir: the direction of the ray
 * @mapBox: pointer to the map coordinates (IntVector)
 * @deltaDist: pointer to delta distance between map lines
 * @stepDir: pointer to step direction (IntVector)
 * @sideDist: pointer to side distance (floatVector)
 */
void step_sidedist(Player *player, floatVector rayDir, IntVector *mapBox,
		floatVector *deltaDist, IntVector *stepDir,
		floatVector *sideDist)
{
	if (rayDir.x < 0)
	{
		stepDir->x = -1;
		sideDist->x = (player->pos.x - mapBox->x) * deltaDist->x;
	}
	else
	{
		stepDir->x = 1;
		sideDist->x = (mapBox->x + 1.0f - player->pos.x) *
			deltaDist->x;
	}
	if (rayDir.y < 0)
	{
		stepDir->y = -1;
		sideDist->y = (player->pos.y - mapBox->y) * deltaDist->y;
	}
	else
	{
		stepDir->y = 1;
		sideDist->y = (mapBox->y + 1.0f - player->pos.y) *
			deltaDist->y;
	}
}

/**
 * perform_dda - perform DDA (Digital Differential Analysis) algorithm
 * @mapBox: pointer to current map box coordinates (IntVector)
 * @stepDir: pointer to step direction (IntVector)
 * @sideDist: pointer to side distance (floatVector)
 * @deltaDist: pointer to delta distance (floatVector)
 * @side: pointer to the side hit (Side enum)
 * Return: true if a wall is hit, false otherwise
 */
bool perform_dda(IntVector *mapBox, IntVector *stepDir, floatVector *sideDist,
		floatVector *deltaDist, Side *side)
{
	if (sideDist->x < sideDist->y)
	{
		sideDist->x += deltaDist->x;
		mapBox->x += stepDir->x;
		*side = EastWest;
	}
	else
	{
		sideDist->y += deltaDist->y;
		mapBox->y += stepDir->y;
		*side = NorthSouth;
	}
	return (MAP[xy2index(mapBox->x, mapBox->y, MAP_SIZE)] > 0);
}


/**
 * calculate_wallx - calculate the wall position (X-coordinate)
 * @player: pointer to the Player struct
 * @rayDir: direction of the ray (floatVector)
 * @perpWallDist: perpendicular wall distance
 * @side: which side was hit (Side enum)
 * Return: the X position of the wall hit
 */
float calculate_wallx(Player *player, floatVector rayDir, float perpWallDist,
		Side side)
{
	if (side == EastWest)
		return (player->pos.y + perpWallDist * rayDir.y);
	else
		return (player->pos.x + perpWallDist * rayDir.x);
}
