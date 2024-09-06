#include "../headers/headers.h"

// #define MIN(x, y) (x < y ? x : y)
// #define MAX(x, y) (x > y ? x : y)

#define MAP_SIZE 16
const uint8_t MAP[MAP_SIZE * MAP_SIZE] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2, 0, 1, 
	1, 0, 0, 0, 0, 0, 3, 2, 2, 2, 2, 2, 2, 2, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

const float playerFOV = (PI / 2.0f);
const float maxDepth = 20.0f;

typedef enum {NorthSouth, EastWest} Side;

int xy2index(int x, int y, int w) {
    return y * w + x;
}

SDL_Texture *wallTexture = NULL;

void render(State *state, Player* player) {
    //loop through each vertical strip of the screen
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
        // the left side of the screen is -1, center is 0 and left is 1
        float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
        //direction of the ray being cast
        floatVector rayDir = {
            .x = player->dir.x + player->plane.x * cameraX,
            .y = player->dir.y + player->plane.y * cameraX,
        };

        // current position on the map squares
        //the ray position is more accurate
        IntVector mapBox = {
            .x = (int)player->pos.x, 
            .y = (int)player->pos.y
        };

        // distance the ray travels from start position to the next x/y-side
        floatVector sideDist = {0.0, 0.0};

        // ditance traveled from current x/y-side to the next
        floatVector deltaDist = {
            .x = (rayDir.x == 0) ? 1e30 : fabsf(1 / rayDir.x), //the 1e30 is to avoid dividing by zero and getting infinite
            .y = (rayDir.y == 0) ? 1e30 : fabsf(1 / rayDir.y),
        };

        //will be used to calculate the length of the ray
        float perpWallDist;
        
        // What direction to step in x- or y-direction (either +1 or -1)
        IntVector stepDir = {0, 0};

        bool hit = false; // was there a wall hit
        Side side; // whether a northsouth/y side or a eastwest/x side was hit

        // if the ray dirx is negative
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

        // start dda
        while (!hit) {
            // jump to next map square
            if (sideDist.x < sideDist.y) {
                sideDist.x += deltaDist.x;
                mapBox.x += stepDir.x;
                side = EastWest; // 0
            } else {
                sideDist.y += deltaDist.y;
                mapBox.y += stepDir.y;
                side = NorthSouth; //1
            }
            // check if ray has hit a wall
            if (MAP[xy2index(mapBox.x, mapBox.y, MAP_SIZE)] > 0) {
                hit = true;
            }
        }

        // Calculate the distance projected on camera direction
        if(side == EastWest)
        {
            perpWallDist = (sideDist.x - deltaDist.x);
        }
        else
        {
            perpWallDist = (sideDist.y - deltaDist.y);
        }
        // switch (side) {
        //     case EastWest:
        //         perpWallDist = (sideDist.x - deltaDist.x);
        //         break;
        //     case NorthSouth:
        //         perpWallDist = (sideDist.y - deltaDist.y);
        //         break;
        // }

        // Calculate height of line to draw on screen 
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        // calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2; 
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT;

        // Calculate texture coordinates
        float wallX; // where the wall was hit
        if (side == EastWest) {
            wallX = player->pos.y + perpWallDist * rayDir.y;
        } else {
            wallX = player->pos.x + perpWallDist * rayDir.x;
        }
        int TEXTURE_WIDTH = 1024;
        int TEXTURE_HEIGHT = 1024;
        wallX -= floorf(wallX); // calculate the exact position within the texture
        int texX = (int)(wallX * (float)TEXTURE_WIDTH); // texture width

        // Draw textured wall
        SDL_Rect srcRect = { texX, 0, 1, TEXTURE_HEIGHT }; // texture coordinates
        SDL_Rect destRect = { x, drawStart, 1, lineHeight }; // screen coordinates
        SDL_RenderCopy(state->renderer, wallTexture, &srcRect, &destRect);
    }
}

int main(void) {
    ASSERT(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO),
           "SDL failed to initialize; %s\n",
           SDL_GetError());

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("Could not initialize SDL_image. Error:%s\n", IMG_GetError());
        return 1;
    }

    State state = {
        .running= true,
    };
    state.window =
        SDL_CreateWindow("Raycast",
                         SDL_WINDOWPOS_CENTERED_DISPLAY(0),
                         SDL_WINDOWPOS_CENTERED_DISPLAY(0),
                         SCREEN_WIDTH,
                         SCREEN_HEIGHT,
                         SDL_WINDOW_ALLOW_HIGHDPI);
    ASSERT(state.window,
           "failed to create SDL window: %s\n",
           SDL_GetError());

    state.renderer =
        SDL_CreateRenderer(state.window,
                           -1,
                           SDL_RENDERER_PRESENTVSYNC);
    ASSERT(state.renderer,
           "failed to create SDL renderer: %s\n",
           SDL_GetError());

    loadTextures(&state, &wallTexture); // Load the texture

    SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
    SDL_SetRelativeMouseMode(true);

    // Initial direction and position and camera plane
    Player player = {
        .pos = {.x =  4.0f, .y =  4.0f},
        .dir = {.x = -1.0f, .y =  0.0f},
        .plane = {.x = 0.0f, .y = 0.66f},
    };

    const float
        rotateSpeed = 0.025,
        moveSpeed = 0.05;

    //the main loop
    while (state.running)
    {
        SDL_Event event;
        int mouse_xrel = 0;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    state.running = false;
                    break;
                case SDL_MOUSEMOTION:
                    mouse_xrel = event.motion.xrel;
                    break;
            }
        }

        const uint8_t* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_ESCAPE]) state.running = false;
        if (mouse_xrel != 0)
        { // rotate
            float rotSpeed = rotateSpeed * (mouse_xrel * -0.1);
            // both camera direction and camera plane must be rotated
            floatVector oldDir = player.dir;
            player.dir.x = player.dir.x * cosf(rotSpeed) - player.dir.y * sinf(rotSpeed);
            player.dir.y = oldDir.x     * sinf(rotSpeed) + player.dir.y * cosf(rotSpeed);

            floatVector oldPlane = player.plane;
            player.plane.x = player.plane.x * cosf(rotSpeed) - player.plane.y * sinf(rotSpeed);
            player.plane.y = oldPlane.x     * sinf(rotSpeed) + player.plane.y * cosf(rotSpeed);
        }

        floatVector deltaPos = {
            .x = player.dir.x * moveSpeed,
            .y = player.dir.y * moveSpeed,
        };
        if (keystate[SDL_SCANCODE_W])
        { // forward
            if (MAP[xy2index(
                        player.pos.x + deltaPos.x, 
                        player.pos.y, 
                        MAP_SIZE)] == 0){
                player.pos.x += deltaPos.x;
            }
            if (MAP[xy2index(
                        player.pos.x, 
                        player.pos.y + deltaPos.y, 
                        MAP_SIZE)] == 0) {
                player.pos.y += deltaPos.y;
            }
        }
        if (keystate[SDL_SCANCODE_S])
        { // backwards
            if (MAP[xy2index(
                        player.pos.x - deltaPos.x, 
                        player.pos.y, 
                        MAP_SIZE)] == 0) {
                player.pos.x -= deltaPos.x;
            }
            if (MAP[xy2index(
                        player.pos.x, 
                        player.pos.y - deltaPos.y, 
                        MAP_SIZE)] == 0) {
                player.pos.y -= deltaPos.y;
            }
        }
        if (keystate[SDL_SCANCODE_D])
        { // turn right
            floatVector oldDir = player.dir;
            player.dir.x = oldDir.x * cosf(-rotateSpeed) - oldDir.y * sinf(-rotateSpeed);
            player.dir.y = oldDir.x * sinf(-rotateSpeed) + oldDir.y * cosf(-rotateSpeed);

            floatVector oldPlane = player.plane;
            player.plane.x = oldPlane.x * cosf(-rotateSpeed) - oldPlane.y * sinf(-rotateSpeed);
            player.plane.y = oldPlane.x * sinf(-rotateSpeed) + oldPlane.y * cosf(-rotateSpeed);

            deltaPos.x = player.plane.x * moveSpeed;
            deltaPos.y = player.plane.y * moveSpeed;

            if (MAP[xy2index(
                        player.pos.x + deltaPos.x, 
                        player.pos.y, 
                        MAP_SIZE)] == 0) {
                player.pos.x += deltaPos.x;
            }
            if (MAP[xy2index(
                        player.pos.x, 
                        player.pos.y + deltaPos.y, 
                        MAP_SIZE)] == 0) {
                player.pos.y += deltaPos.y;
            }
        }
        if (keystate[SDL_SCANCODE_A])
        { // turn left
            floatVector oldDir = player.dir;
            player.dir.x = oldDir.x * cosf(rotateSpeed) - oldDir.y * sinf(rotateSpeed);
            player.dir.y = oldDir.x * sinf(rotateSpeed) + oldDir.y * cosf(rotateSpeed);

            floatVector oldPlane = player.plane;
            player.plane.x = oldPlane.x * cosf(rotateSpeed) - oldPlane.y * sinf(rotateSpeed);
            player.plane.y = oldPlane.x * sinf(rotateSpeed) + oldPlane.y * cosf(rotateSpeed);

            deltaPos.x = player.plane.x * moveSpeed;
            deltaPos.y = player.plane.y * moveSpeed;

            if (MAP[xy2index(
                        player.pos.x - deltaPos.x, 
                        player.pos.y, 
                        MAP_SIZE)] == 0) {
                player.pos.x -= deltaPos.x;
            }
            if (MAP[xy2index(
                        player.pos.x, 
                        player.pos.y - deltaPos.y, 
                        MAP_SIZE)] == 0) {
                player.pos.y -= deltaPos.y;
            }
        }

        SDL_RenderClear(state.renderer);
        render(&state, &player);
        SDL_RenderPresent(state.renderer);
    }

    SDL_DestroyTexture(wallTexture);
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
