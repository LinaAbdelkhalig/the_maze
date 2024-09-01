#ifndef RAYCASTING_HPP
#define RAYCASTING_HPP

#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>

void renderWalls(SDL_Renderer *gRenderer, int sWidth, int sHeight, int wMap[8][8], int mWidth, int mHeight, float camX, float camY, float camAngle);

#endif /* RAYCASTING_HPP */