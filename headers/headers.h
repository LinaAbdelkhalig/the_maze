#ifndef HEADERS_H
#define HEADERS_H

/* Standard Libraries */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* SDL2 Libraries */
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

/* global variables */
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PI 3.14159265f
extern uint8_t *MAP;
extern int MAP_SIZE;
extern SDL_Texture *weaponTexture;
extern SDL_Texture *weaponShotTexture;
extern Mix_Chunk *walkSFX;
extern Mix_Chunk *rainSFX;
extern Mix_Chunk *gunSFX;
extern SDL_Texture *textureArray[5];

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
 * @miniMap: Flag indicating if the mini-map is displayed.
 */
typedef struct State
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool running;
	bool raining;
	bool miniMap;
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

/**
 * enum Side - The direction the wall is facing.
 *
 * @NorthSouth: Whether the wall is facing north or south.
 * @EastWest: Whether the wall is facing east or west.
 *
 * This enum is used to determine the direction the wall is facing
 * to determine whether it will be shaded or not.
 */
typedef enum Side
{
	NorthSouth,
	EastWest
} Side;

/* Macros */
#define ASSERT(cond, ...) \
	do { \
		if (!(cond)) \
		{ \
			fprintf(stderr, __VA_ARGS__); \
			exit(1); \
		} \
	} while (0)

/* Function declarations */
/* Initialization functions */
void init(State *state);
void clean_up(State *state, SDL_Texture *textureArray[]);
void init_textures(SDL_Texture *textureArray[], State *state);
void load_sfx(void);
void init_textures(SDL_Texture *textureArray[], State *state);
void loadTextures(State *state, SDL_Texture **texture, const char *path);

/* Input handling functions */
int handle_events(State *state, bool *isKilling);
void update_playerpos(floatVector *deltaPos, Player *player,
		const uint8_t *keystate, float moveSpeed);
void mouse_rotation(int *mouse_xrel, float rotateSpeed, Player *player);
void key_rotation_right(const uint8_t *keystate, Player *player,
		floatVector *deltaPos, Mix_Chunk *walkSFX,
		float rotateSpeed, float moveSpeed);
void key_rotation_left(const uint8_t *keystate, Player *player,
		floatVector *deltaPos, Mix_Chunk *walkSFX,
		float rotateSpeed, float moveSpeed);

/* Map functions */
void getMap(const char *fp);
int xy2index(int x, int y, int w);

/* Rendering functions */
void render_scene(State *state, Player *player);
void renderFloor(State *state, Player *player, int texWidth,
		int texHeight, SDL_Texture **flrTexture);
void renderCeiling(State *state, Player *player,
		int texWidth, int texHeight, SDL_Texture **clngTexture);
void renderWall(State *state, Player *player, int texWidth,
		int texHeight, SDL_Texture **wallTexture);
void renderRain(State *state);
void renderWeapon(State *state, bool isKilling);
void renderMiniMap(State *state, Player *player, int miniMapSize);

/* Calculation functions */
floatVector calculate_raydir(Player *player, float cameraX);
void step_sidedist(Player *player, floatVector rayDir,
		IntVector *mapBox, floatVector *deltaDist,
		IntVector *stepDir, floatVector *sideDist);
bool perform_dda(IntVector *mapBox, IntVector *stepDir,
		floatVector *sideDist, floatVector *deltaDist,
		Side *side);
float calculate_wallx(Player *player, floatVector rayDir,
		float perpWallDist, Side side);

#endif /* HEADERS_H */
