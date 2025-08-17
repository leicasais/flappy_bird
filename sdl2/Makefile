# ====== Config ======
APP      := flappy
BUILD    := build
BIN      := $(BUILD)/$(APP)

CC       := gcc
CSTD     := -std=c11
WARN     := -Wall -Wextra -Wshadow -Wconversion
OPT      := -O2
DEPFLAGS := -MMD -MP

SRC := \
  main.c \
  init_fun.c \
  frontend.c \
  bird_fun.c \
  column_fun.c \
  game_logic.c \
  menu.c

OBJ := $(patsubst %.c,$(BUILD)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

# ====== SDL2 detection ======
PKG_OK := $(shell pkg-config --exists sdl2 SDL2_image SDL2_ttf && echo yes || echo no)
ifeq ($(PKG_OK),yes)
  SDL_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
  SDL_LIBS   := $(shell pkg-config --libs   sdl2 SDL2_image SDL2_ttf)
else
  # Fallback: typical on some macOS set-ups
  SDL_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
  SDL_LIBS   := $(shell sdl2-config --libs   2>/dev/null) -lSDL2_image -lSDL2_ttf
endif

CFLAGS := $(CSTD) $(WARN) $(OPT) $(DEPFLAGS) $(SDL_CFLAGS)
LDFLAGS := $(SDL_LIBS)

# ====== Rules ======
.PHONY: all run clean distclean

all: $(BIN)

$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

run: $(BIN)
	# Ejecutamos desde build/ porque el código carga assets con rutas ../img/...
	cd $(BUILD) && ./$(APP)

clean:
	@rm -f $(OBJ) $(DEP)

distclean: clean
	@rm -f $(BIN)

-include $(DEP)
