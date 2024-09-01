#include "../headers/raycasting.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// 1 is a wall and the 0 are empt spaces
int wMap[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};

// Camera positions
float camX = 3.5f;
float camY = 3.5f;
float camAngle = 5.0f;

// Close and free resources
void close(SDL_Window *gWindow, SDL_Renderer *gRenderer)
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char *argv[])
{
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Could not initialize SDL. Error:%s\n", SDL_GetError());
        return 0;
    }

    // Create Window
    SDL_Window *gWindow = SDL_CreateWindow("THe Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!gWindow)
    {
        printf("Could not create window. Error:%s\n", SDL_GetError());
        return 0;
    }

    // Render. -1 the first rendering driver that supports the flags
    SDL_Renderer *gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if(!gRenderer)
    {
        printf("Could not create rendering context. Error:%s\n", SDL_GetError());
        return 0;
    }

    // Set up the main loop
    bool running = true;
    SDL_Event e;

    while(running)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Color the sky rgba
        SDL_SetRenderDrawColor(gRenderer, 135, 206, 235, 255);

        // Clear the gRenderer with the drawing color
        SDL_RenderClear(gRenderer);
        // maybe add error here?

        // Draw the walls
        renderWalls(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, wMap, 8, 8, camX, camY, camAngle);

        // Present the backbuffer to the screen
        SDL_RenderPresent(gRenderer);
    }

    close(gWindow, gRenderer);

    return 0;
}