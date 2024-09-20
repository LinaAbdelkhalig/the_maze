# compiler
CC = gcc

#compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic

#the sdl2-config command doesnt handle linking SDL2_image
SDLFLAGS = `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer

#files to compile
SRC = $(wildcard src/*.c)

#math flags
MATHFLAGS = -lm

OBJ = $(patsubst %.c, %.o, $(SRC))

TARGET = maze

all: $(TARGET)

# linking
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(SDLFLAGS) $(MATHFLAGS)

# compilation step for each .c file to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
