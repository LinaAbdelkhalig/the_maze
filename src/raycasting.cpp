#include "../headers/raycasting.hpp"

void renderWalls(SDL_Renderer *gRenderer, int SCREEN_WIDTH, int SCREEN_HEIGHT, int wMap[8][8], int mWidth, int mHeight, float camX, float camY, float camAngle)
{
    // Field of view
    float fov = 60.0f * (M_PI / 180.0f);
    float halfFov = fov / 2.0f;

    // Drawing the walls using raycasting
    for(int x = 0; x < SCREEN_WIDTH; x++)
    {
        float rayAngle = (camAngle - halfFov) + ((float)x / (float)SCREEN_WIDTH) *fov;

        float rayX = cos(rayAngle);
        float rayY = sin(rayAngle);


        int mapX = (int)camX;
        int mapY = (int)camY;

        float deltaDistX = fabs(1 / rayX);
        float deltaDistY = fabs(1 / rayY);
        float sideDistX;
        float sideDistY;
        float perpWallDist;

        int stepX;
        int stepY;

        if (rayX < 0)
        {
            stepX = -1;
            sideDistX = (camX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0f - camX) * deltaDistX;
        }

        if(rayY < 0)
        {
            stepY = -1;
            sideDistY = (camY - mapY) + deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0f - camY) * deltaDistY;
        }

        bool collision = false;
        bool side;
        while(!collision)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += sideDistX;
                mapX += stepX;
                side = false;
            }
            else
            {
                sideDistY += sideDistY;
                mapY += stepY;
                side = true;
            }

            if(wMap[mapX][mapY] > 0)
            {
                collision = true;
            }
        }

        if(side)
        {
            perpWallDist = (mapY - camY + (1 - stepY) / 2) / rayY;
        }
        else
        {
            perpWallDist = (mapX - camX + (1 - stepX) / 2) / rayX;
        }

        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if(drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // Color the ground
        SDL_SetRenderDrawColor(gRenderer, 0, 128, 0, 255);
        SDL_Rect ground = {0, drawEnd, SCREEN_WIDTH, SCREEN_HEIGHT - drawEnd};
        SDL_RenderFillRect(gRenderer, &ground);

        // Color the walls
        SDL_SetRenderDrawColor(gRenderer, 77, 77, 77, 1);
        SDL_RenderDrawLine(gRenderer, x, drawStart, x, drawEnd);
    }
}