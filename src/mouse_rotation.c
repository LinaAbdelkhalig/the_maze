#include "../headers/headers.h"
/**
 * mouse_rotation - handles the rotation with mouse movement
 * @mouse_xrel: mouse relative horizontal movement
 * @rotateSpeed: rotation speed
 * @player: pointer to the player struct
 *
 * Return: void
 */
void mouse_rotation(int *mouse_xrel, float rotateSpeed, Player *player)
{
	/* if mouse movement is detected*/
	if (*mouse_xrel != 0)
	{
		float rotSpeed = rotateSpeed * ((*mouse_xrel) * -0.1);

		floatVector oldDir = player->dir;

		/* rotate the player direction using the 2d rotation matrix */
		player->dir.x = player->dir.x * cosf(rotSpeed) -
			player->dir.y * sinf(rotSpeed);
		player->dir.y = oldDir.x * sinf(rotSpeed) +
			player->dir.y * cosf(rotSpeed);

		/* rotate the camera plane */
		floatVector oldPlane = player->plane;

		player->plane.x = player->plane.x * cosf(rotSpeed) -
			player->plane.y * sinf(rotSpeed);
		player->plane.y = oldPlane.x * sinf(rotSpeed) +
			player->plane.y * cosf(rotSpeed);
	}
}
