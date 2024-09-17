#include "../headers/headers.h"

uint8_t *MAP = NULL;
int MAP_SIZE = 0;

/**
 * openFile - Opens the map file.
 *
 * @fp: The path of the file with the map.
 *
 * Return: The open file.
 */
FILE *openFile(const char *fp)
{
	FILE *file = fopen(fp, "r");

	ASSERT(file, "Cannot open the map file: %s\n", strerror(errno));
	return (file);
}

/**
 * closeFile - Free resources and close the map file.
 *
 * @file: The file to be closed.
 * @line: The line to be freed.
 */
void closeFile(FILE *file, char *line)
{
	if (line)
		free(line);
	fclose(file);
}

/**
 * getMapSize - Gets the map size from the file.
 *
 * @file: The open map file
 *
 * Return: The map size.
 */
int getMapSize(FILE *file)
{
	int w = 0, h = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	/* Get the width and height of the file */
	read = getline(&line, &len, file);
	ASSERT((read != -1), "Map file is empty.\n");
	w = (int)strlen(line) - 1;
	fseek(file, 0, SEEK_SET);
	while ((read = getline(&line, &len, file)) != -1)
		++h;
	free(line); /* Free the allocated memory */
	fseek(file, 0, SEEK_SET);

	/* Check if there is a square map or not */
	if (w == 0 || h == 0 || w != h)
	{
		fclose(file);
		ASSERT(false, "Map file is not square or is empty.\n");
	}
	return (h);
}

/**
 * getMap - Gets the map from a file.
 *
 * @fp: The path of the file with the map.
 */
void getMap(const char *fp)
{
	FILE *file;
	char c;
	int x = 0, y = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	/* Open the map file */
	file = openFile(fp);
	MAP_SIZE = getMapSize(file); /* Get the map size from the file */
	MAP = (uint8_t *)malloc(MAP_SIZE * MAP_SIZE); /* Allocate map memory */
	ASSERT(MAP, "Failed to allocate memory for the map.\n");
	for (; (read = getline(&line, &len, file)) != -1 && y < MAP_SIZE; ++y)
	{/* Read the map from the file */
		for (x = 0; x < MAP_SIZE; ++x)
		{
			c = line[x];
			switch (c)
			{
				case '1':
					MAP[xy2index(x, y, MAP_SIZE)] = 1;
					break;
				case '0':
					MAP[xy2index(x, y, MAP_SIZE)] = 0;
					break;
				case '2':
					MAP[xy2index(x, y, MAP_SIZE)] = 2;
					break;
				case '3':
					MAP[xy2index(x, y, MAP_SIZE)] = 3;
					break;
				case '\n':
					continue;
				default:
					closeFile(file, line);
					ASSERT(false, "The map can only contain 0, 1, 2, or 3.\n");
			}
		}
	}
	closeFile(file, line);
}
