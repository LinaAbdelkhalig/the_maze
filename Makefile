# compiler
CC = g++

#compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic

SDLFLAGS = `sdl2-config --cflags --libs`

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