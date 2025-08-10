CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lncurses -lSDL2

SRC = main.c backend.c frontend.c
OBJ = $(SRC:.c=.o)
DEPS = backend.h frontend.h

TARGET = flappy_bird

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean