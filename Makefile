CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm -lraylib

SRC_DIR = src
BUILD_DIR = compiled

WOLF1_DIR = $(SRC_DIR)/semi3d/wolfenstein/Woolf1
WOLF2_DIR = $(SRC_DIR)/semi3d/wolfenstein/Woolf2
DOOM_DIR = $(SRC_DIR)/semi3d/doom/doom
MAPMAKER_DIR = $(SRC_DIR)/semi3d/doom/mapmaker

WOLF1_SRC = $(wildcard $(WOLF1_DIR)/*.c)
WOLF2_SRC = $(wildcard $(WOLF2_DIR)/*.c)
DOOM_SRC = $(wildcard $(DOOM_DIR)/*.c)
MAPMAKER_SRC = $(wildcard $(MAPMAKER_DIR)/*.c)

WOLF1_BIN = $(BUILD_DIR)/Woolf1.out
WOLF2_BIN = $(BUILD_DIR)/Woolf2.out
DOOM_BIN = $(BUILD_DIR)/Doom1.out
MAPMAKER_BIN = $(BUILD_DIR)/Mapmaker.out

all: woolf1 woolf2 doom mapmaker

woolf1: $(WOLF1_BIN)
woolf2: $(WOLF2_BIN)
doom: $(DOOM_BIN)
mapmaker: $(MAPMAKER_BIN)

$(WOLF1_BIN): $(WOLF1_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(WOLF2_BIN): $(WOLF2_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(DOOM_BIN): $(DOOM_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(MAPMAKER_BIN): $(MAPMAKER_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(BUILD_DIR)/*.out

.PHONY: all woolf1 woolf2 doom mapmaker clean

