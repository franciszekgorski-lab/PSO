COMP = gcc
COMP_FLAGS = -Wall -Wextra -Iinclude $(shell pkg-config --cflags sdl2)
LD_FLAGS = $(shell pkg-config --libs sdl2)

TARGET = PSO
SRC = src/main.c src/logger.c src/map.c src/pso.c src/utils.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(COMP) $(OBJ) -o $@ $(LD_FLAGS) -lSDL2

%.o: %.c
	$(COMP) $(COMP_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
