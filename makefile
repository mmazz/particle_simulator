CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude     # <--- Le indicamos que busque headers en include/
LIBS = -lglfw -lGLEW -lGL -lm

SRC = src/main.c src/renderer.c src/particle.c src/shader.c src/utils.c
OBJ = $(SRC:.c=.o)

all: simulator

simulator: $(OBJ)
	$(CC) $(CFLAGS) -o build/$@ $^ $(LIBS)

clean:
	rm -f src/*.o build/simulator
