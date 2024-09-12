# compiler
CC = gcc

#compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic

#the sdl2-config command doesnt handle linking SDL2_image
SDLFLAGS = `sdl2-config --cflags --libs` -lSDL2_image

#files to compile
SRC = src/main.c src/load_textures.c src/rain.c src/get_map.c src/render.c src/weapons.c

#math flags
MATHFLAGS = -lm

OBJ = $(SRC:.c=.o)

TARGET = maze

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(SDLFLAGS) $(MATHFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
