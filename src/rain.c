#include "../headers/headers.h"

/**
 * renderRain - Renders the rain effect.
 *
 * @state: Pointer to the game state.
 */
void renderRain(State *state)
{
	/* Set the drawing color for raindrops to transparent light blue */
	SDL_SetRenderDrawColor(state->renderer, 173, 216, 230, 150);

	/* The loop is the number of drops in the screen */
	for (int i = 0; i < 100; i++)
	{
		/* The drop x and y coordinates */
		int x = rand() % SCREEN_WIDTH;
		int y = rand() % SCREEN_HEIGHT;
		/* Random length for the raindrop */
		int len = rand() % 10 + 5;

		/* Draw the raindrops */
		SDL_RenderDrawLine(state->renderer, x, y, x, y + len);
	}

	/* Reset the color to black to avoid affecting other render calls */
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
}
