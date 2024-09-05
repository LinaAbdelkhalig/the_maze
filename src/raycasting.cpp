#include "../headers/raycasting.hpp"

/**
 * renderWalls - This function draws wall in the 3D maze
 *
 * @gRenderer: SDL renderer used to draw the walls.
 * @SCREEN: Struct holding screen dimensions.
 * @wMap: Array that represents the map of walls.
 * @mWidth: The width of the map in cells.
 * @mHeight: The height of the map in cells.
 * @player: Struct holding player position information.
 */
void renderWalls(SDL_Renderer *gRenderer, Screen SCREEN, SDL_Texture *wallTexture, int wMap[8][8], int mWidth, int mHeight, Player player)
{
    // Field of view
    float fov = 60.0f * (M_PI / 180.0f);
    float halfFov = fov / 2.0f;

    // Drawing the walls using raycasting
    for(int x = 0; x < SCREEN.WIDTH; x++) // For the width of the screen
    {
        //                  starting leftmost angle     position of the current column relative to screen width
        float rayAngle = (player.camAngle - halfFov) + ((float)x / (float)SCREEN.WIDTH) * fov;
        // multiplying the above by pov scales the fractional position across the entire field of view
        float rayX = cos(rayAngle);
        float rayY = sin(rayAngle);

        int mapX = (int)player.x;
        int mapY = (int)player.y;

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
            sideDistX = (player.x - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0f - player.x) * deltaDistX;
        }

        if(rayY < 0)
        {
            stepY = -1;
            sideDistY = (player.y - mapY) + deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0f - player.y) * deltaDistY;
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
            perpWallDist = (mapY - player.y + (1 - stepY) / 2) / rayY;
        }
        else
        {
            perpWallDist = (mapX - player.x + (1 - stepX) / 2) / rayX;
        }

        int lineHeight = (int)(SCREEN.HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + SCREEN.HEIGHT / 2;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN.HEIGHT / 2;
        if(drawEnd >= SCREEN.HEIGHT) drawEnd = SCREEN.HEIGHT - 1;

        // Color the ground
        SDL_SetRenderDrawColor(gRenderer, 0, 128, 0, 255);
        SDL_Rect ground = {0, drawEnd, SCREEN.WIDTH, SCREEN.HEIGHT - drawEnd};
        SDL_RenderFillRect(gRenderer, &ground);

        // Color the walls
        // if(side) // East or West facing walls
        // {
        //     SDL_SetRenderDrawColor(gRenderer, 112, 112, 112, 255);
        // }
        // else // North or South facing walls
        // {
        //     SDL_SetRenderDrawColor(gRenderer, 90, 90, 90, 255);
        // }
        // SDL_RenderDrawLine(gRenderer, x, drawStart, x, drawEnd);
        // Calculate texture X coordinate
        float wallX; // Exact x-coordinate on the wall
        if(side == 0) wallX = player.y + perpWallDist * rayY;
        else          wallX = player.x + perpWallDist * rayX;
        wallX -= floor(wallX);

        // X coordinate on the texture
        int TEXTURE_WIDTH = 1024;
        int TEXTURE_HEIGHT = 1024;
        int texX = (int)(wallX * (float)TEXTURE_WIDTH);
        if(side == 0 && rayX > 0) texX = TEXTURE_WIDTH - texX - 1;
        if(side == 1 && rayY < 0) texX = TEXTURE_WIDTH - texX - 1;

        // Set up the source rectangle
        SDL_Rect srcRect;
        srcRect.x = texX;
        srcRect.y = 0;        
        srcRect.w = 1;        
        srcRect.h = TEXTURE_HEIGHT;        

        // Set up the destination rectangle
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = drawStart;
        destRect.w = 1;
        destRect.h = drawEnd - drawStart;

        // Render the textured wall
        SDL_RenderCopy(gRenderer, wallTexture, &srcRect, &destRect);
    }
}
