#ifndef RAYCASTING_HPP
#define RAYCASTING_HPP

#include <cmath>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

/**
 * struct Screen - screen dimensions
 * @WIDTH: width of the screen
 * @HEIGHT: height of the screen
 *
 * Description: holds information about the window/screen dimensions
 */
struct Screen
{
    const int WIDTH {1280};
    const int HEIGHT {720};
};

/**
 * struct Player - player/camera information
 * @x: position of the player on the x axis (column)
 * @y: position of the player on the y axis (row)
 * @camAngle: the angle of the camera in degrees
 *
 * Description: information about the player's position on the map
 */
struct Player
{
    float x {6.0f};
    float y {1.0f};
    float camAngle {90.0f};
};

void renderWalls(SDL_Renderer *gRenderer, Screen SCREEN, SDL_Texture *wallTexture, int wMap[8][8], int mWidth, int mHeight, Player player);

#endif /* RAYCASTING_HPP */