#include "../headers/raycasting.hpp"

Screen SCREEN {1280, 720};

// 1 is a wall and the 0 are empt spaces
int wMap[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};

// Player starting positions
Player player {6.0f, 1.0f, 90.0f};

// Camera rotation speed and movement speed
const float rotSpeed = M_PI / 4;
const float camSpeed = 0.05f;

/**
 * checkCollision - checks for collision with walls.
 *
 * @x: The x-coordinate of the player position.
 * @y: The y-coordinate of the camera position.
 *
 * Return: True if there is collision with walls, false if not.
 */
bool checkCollision(float x, float y)
{
    int mapX = (int)x;
    int mapY = (int)y;

    // In case it hits the map boundries
    if (mapX < 0 || mapX >= 8 || mapY < 0 || mapY >= 8)
        return true;

    // True if there is a wall collision, False if no wall
    return wMap[mapX][mapY] > 0;
}

/**
 * playerMove - Handles the player movements and collision.
 *
 * @diffX: The change in x-coordinate of the player.
 * @diffY: The change in y-coordinate of the player.
 */
void playerMove(float diffX, float diffY)
{
    // The next move coordinates
    float nextX = player.x + diffX;
    float nextY = player.y + diffY;

    // Check collision
    if (!checkCollision(nextX, nextY))
    {
        player.x = nextX;
        player.y = nextY;
    }
    else
    {
        // Slide along the walls (horizontal movement)
        // Sliding coordinates
        float slideX = diffX;
        float slideY = 0.0f;
        if (checkCollision(player.x + slideX, player.y + slideY))
        {
            player.x += slideX;
            player.y += slideY;
        }
        // Slide along the walls (vertical movement)
        else
        {
            // Sliding coordinates
            slideX = 0.0f;
            slideY = diffY;
            if (checkCollision(player.x + slideX, player.y + slideY))
            {
                player.x += slideX;
                player.y += slideY;
            }
        }
    }
}

/**
 * close - Close and free SDL resources
 *
 * @gWindow: Pointer to the SDL_Window to be destroyed.
 * @gRenderer: Pointer to the SDL_Renderer to be destroyted.
 */
void close(SDL_Window* gWindow, SDL_Renderer* gRenderer)
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

/**
 * main - Entry point of the application.
 *
 * @argc: Number of command-line arguments.
 * @argv: Command-line arguments.
 *
 * Return: Exit status, 0 for success, 1 for failure.
 */
int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Could not initialize SDL. Error:%s\n", SDL_GetError());
        return 1;
    }

    // Create Window
    SDL_Window* gWindow = SDL_CreateWindow("The Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN.WIDTH, SCREEN.HEIGHT, SDL_WINDOW_SHOWN);
    if (!gWindow)
    {
        printf("Could not create window. Error:%s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Render. -1 the first rendering driver that supports the flags
    SDL_Renderer* gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if (!gRenderer)
    {
        printf("Could not create rendering context. Error:%s\n", SDL_GetError());
        // Destroy and quit the created window due to the rendering failure.
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
        return 1;
    }

    // Set mouse movement on
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Set up the main loop
    bool running = true;
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            // User presses a key
            else if (e.type == SDL_KEYDOWN)
            {
                // Player's next move coordinates
                float moveX = 0.0f, moveY = 0.0f;

                // Adjust the camera angle left or right based on keyboard
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                    player.camAngle -= rotSpeed;
                    break;
                case SDLK_RIGHT:
                    player.camAngle += rotSpeed;
                    break;
                case SDLK_w:
                    moveX = cos(player.camAngle) * camSpeed;
                    moveY = sin(player.camAngle) * camSpeed;
                    playerMove(moveX, moveY);
                    break;
                case SDLK_s:
                    moveX = -cos(player.camAngle) * camSpeed;
                    moveY = -sin(player.camAngle) * camSpeed;
                    playerMove(moveX, moveY);
                    break;
                case SDLK_a:
                    moveX = sin(player.camAngle) * camSpeed;
                    moveY = -cos(player.camAngle) * camSpeed;
                    playerMove(moveX, moveY);
                    break;
                case SDLK_d:
                    moveX = -sin(player.camAngle) * camSpeed;
                    moveY = cos(player.camAngle) * camSpeed;
                    playerMove(moveX, moveY);
                    break;
                default:
                    break;
                }
            }
            // User move the mouse
            else if (e.type == SDL_MOUSEMOTION)
            {
                // Adjust the camera angle left or right based on mouse movements
                player.camAngle += e.motion.xrel * (rotSpeed / 10.0f);
                // TODO: avoid 360 degree rotation
            }
        }

        // Color the sky rgba
        SDL_SetRenderDrawColor(gRenderer, 135, 206, 235, 255);

        // Clear the gRenderer with the drawing color
        SDL_RenderClear(gRenderer);
        // maybe add error here?

        // Draw the walls
        renderWalls(gRenderer, SCREEN, wMap, 8, 8, player);

        // Present the backbuffer to the screen
        SDL_RenderPresent(gRenderer);
    }

    close(gWindow, gRenderer);

    return 0;
}
