#include "../headers/headers.h"

/**
 * key_rotation_right - handles the rotation using keyboard keys
 * @keystate: pointer to array of key states
 * @player: pointer to the player struct
 * @deltaPos: pointer to vector of change in player direction
 * @walkSFX: walking sound effect
 * @rotateSpeed: rotation speed
 * @moveSpeed: movement speed
 *
 * Return: void
 */
void key_rotation_right(const uint8_t *keystate, Player *player,
		floatVector *deltaPos, Mix_Chunk *walkSFX,
		float rotateSpeed, float moveSpeed)
{
	if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT])
	{
		/* turn right */
		floatVector oldDir = player->dir;
		/* rotate the vector with the rotation matrix*/
		player->dir.x = oldDir.x * cosf(-rotateSpeed) - oldDir.y *
			sinf(-rotateSpeed);
		player->dir.y = oldDir.x * sinf(-rotateSpeed) + oldDir.y *
			cosf(-rotateSpeed);

		/* rotate the camera plane */
		floatVector oldPlane = player->plane;

		player->plane.x = oldPlane.x * cosf(-rotateSpeed) -
			oldPlane.y * sinf(-rotateSpeed);
		player->plane.y = oldPlane.x * sinf(-rotateSpeed) +
			oldPlane.y * cosf(-rotateSpeed);
		deltaPos->x = player->plane.x * moveSpeed;
		deltaPos->y = player->plane.y * moveSpeed;
		if (MAP[xy2index(player->pos.x + deltaPos->x,
					player->pos.y,
					MAP_SIZE)] == 0)
			player->pos.x += deltaPos->x;
		if (MAP[xy2index(player->pos.x,
					player->pos.y + deltaPos->y,
					MAP_SIZE)] == 0)
			player->pos.y += deltaPos->y;
		Mix_PlayChannel(0, walkSFX, 0);
	}
}

/**
 * key_rotation_right - handles the rotation using keyboard keys
 * @keystate: pointer to array of key states
 * @player: pointer to the player struct
 * @deltaPos: pointer to vector of change in player direction
 * @walkSFX: walking sound effect
 * @rotateSpeed: rotation speed
 * @moveSpeed: movement speed
 *
 * Return: void
 */
void key_rotation_left(const uint8_t *keystate, Player *player,
		floatVector *deltaPos, Mix_Chunk *walkSFX,
		float rotateSpeed, float moveSpeed)
{
	if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT])
	{
		/* turn left */
		floatVector oldDir = player->dir;
		/* rotate the vector with the rotation matrix*/
		player->dir.x = oldDir.x * cosf(rotateSpeed) - oldDir.y *
			sinf(rotateSpeed);
		player->dir.y = oldDir.x * sinf(rotateSpeed) + oldDir.y *
			cosf(rotateSpeed);

		/* rotate the camera plane */
		floatVector oldPlane = player->plane;

		player->plane.x = oldPlane.x * cosf(rotateSpeed) -
			oldPlane.y * sinf(rotateSpeed);
		player->plane.y = oldPlane.x * sinf(rotateSpeed) +
			oldPlane.y * cosf(rotateSpeed);
		deltaPos->x = player->plane.x * moveSpeed;
		deltaPos->y = player->plane.y * moveSpeed;
		if (MAP[xy2index(player->pos.x + deltaPos->x,
					player->pos.y,
					MAP_SIZE)] == 0)
			player->pos.x -= deltaPos->x;
		if (MAP[xy2index(player->pos.x,
					player->pos.y + deltaPos->y,
					MAP_SIZE)] == 0)
			player->pos.y -= deltaPos->y;

		Mix_PlayChannel(0, walkSFX, 0);
	}
}
