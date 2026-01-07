COMP = gcc
COMP_FLAGS = -Wall -Wextra -Iinclude $(shell pkg-config --cflags sdl2)
LD_FLAGS = -lm $(shell pkg-config --libs sdl2)

TARGET = PSO
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(COMP) $(OBJ) -o $@ $(LD_FLAGS) -lSDL2 -lSDL2_ttf

%.o: %.c
	$(COMP) $(COMP_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
