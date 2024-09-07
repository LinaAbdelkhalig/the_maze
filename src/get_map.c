#include "../headers/headers.h"

uint8_t *MAP = NULL;
int MAP_SIZE = 0;

/**
 * getMap - Gets the map from a file.
 *
 * @fp: The path of the file with the map.
 */
void getMap(const char *fp)
{
	FILE *file;
	char c;
	int x = 0, y = 0, w = 0, h = 0;
	char *line;
	size_t len = 0;
	ssize_t read;

	/* Open the map file */
	file = fopen(fp, "r");
	ASSERT(file, "Cannot open the map file: %s\n", strerror(errno));

	// TODO: Get the width and height of the file
	read = getline(&line, &len, file);
	ASSERT((read != -1), "Map is file is empty.\n");
	w = (int)strlen(line) - 1;
	fseek(file, 0, SEEK_SET);
	while (getline(&line, &len, file) != -1)
	{
		++h;
	}
	free(line);
	fseek(file, 0, SEEK_SET);

	/* Check if there is a square map or not */
	if (w == 0 || h == 0 || w != h)
	{
		fclose(file);
		ASSERT(false, "Map file is empty\n");
	}

	/* Allocate memory for the map */
	MAP_SIZE = h;
	MAP = (uint8_t *)malloc(w * h);
	ASSERT(MAP, "Failed to allocate memory for the map.\n");

	/* Go back to the beginning of the file */
	fseek(file, 0, SEEK_SET);

	/* Read the map from the file */
	for (; y < MAP_SIZE && c != EOF; ++y)
	{
		for (; x < MAP_SIZE; ++x)
		{
			c = fgetc(file);
			switch (c)
			{
				case '1': MAP[xy2index(x, y, MAP_SIZE)] = 1; break;
				case '0': MAP[xy2index(x, y, MAP_SIZE)] = 0; break;
				case '2': MAP[xy2index(x, y, MAP_SIZE)] = 2; break;
				case '3': MAP[xy2index(x, y, MAP_SIZE)] = 3; break;
				case '\n': continue;
				default:
					fclose(file);
					ASSERT(false, "The map can only be 0, 1, 2, 3");
			}
		}
	}
	fclose(file);
}
