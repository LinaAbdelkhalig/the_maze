# compiler
CC = g++

#compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic

#the sdl2-config command doesnt handle linking SDL2_image
SDLFLAGS = `sdl2-config --cflags --libs` -lSDL2_image

#files to compile
SRC = src/main.cpp src/raycasting.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = maze

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(SDLFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean