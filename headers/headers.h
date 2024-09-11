#ifndef HEADERS_H
#define HEADERS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* Constants */
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define PI 3.14159265f

/* Global variables */
extern uint8_t *MAP;
extern int MAP_SIZE;

/* Structs */
/**
 * struct floatVector - The vector with float x and y coordinates.
 *
 * @x: The x-coordinate.
 * @y: The y-coordinate.
 */
typedef struct floatVector
{
	float x, y;
} floatVector;

/**
 * struct IntVector - The vector with integer x and y coordinates.
 *
 * @x: The x-coordinate.
 * @y: The y-coordinate.
 */
typedef struct IntVector
{
	int x, y;
} IntVector;

/**
 * struct State - The state of the game.
 *
 * @window: The SDL window.
 * @renderer: The SDL renderer.
 * @running: Flag indicating if the game is running.
 * @raining: Flag indicating if it is raining.
 */
typedef struct State
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool running;
	bool raining;
} State;

/**
 * struct Player - The game player.
 *
 * @pos: The position of the player.
 * @dir: The direction the player is facing.
 * @plane: The plane of the player's view.
 */
typedef struct Player
{
	floatVector pos;
	floatVector dir;
	floatVector plane;
} Player;

/* Functions */
#define ASSERT(cond, ...) \
	do { \
		if (!cond) \
		{ \
			fprintf(stderr, __VA_ARGS__); \
			exit(1); \
		} \
	} while (0)

/* Function declarations */
void loadTextures(State *state, SDL_Texture **wallTexture);
void renderRain(State *state);
void getMap(const char *fp);
int xy2index(int x, int y, int w);

#endif /* HEADERS_H */
