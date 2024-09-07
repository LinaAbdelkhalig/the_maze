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

//vars
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define PI 3.14159265f

/* global variables */
extern uint8_t *MAP;
extern int MAP_SIZE;

// structs
typedef struct {
    float x, y;
} floatVector;

typedef struct {
    int x, y;
} IntVector;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running;
	bool raining;
} State;

typedef struct {
	floatVector pos;
	floatVector dir;
    floatVector plane;
} Player;

//functions
#define ASSERT(cond, ...)                                                      \
  if (!cond) {                                                                 \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1);                                                                   \
  }

void loadTextures(State *state, SDL_Texture **wallTexture);
void renderRain(State* state);
void getMap(const char *fp);
// void drawMap(State *state, Player *player0;
int xy2index(int x, int y, int w);

#endif /* HEADERS_H */
