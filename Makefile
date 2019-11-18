BIN_NAME := client

SRC_PATH := src
BUILD_PATH := build
BIN_PATH := bin

COMPILE_FLAGS := -g

SRC_EXT := c
CC := gcc

NCURSES_CFLAGS := -D_GNU_SOURCE -D_DEFAULT_SOURCE -I/usr/include/ncursesw
NCURSES_LIBS := -lncursesw -ltinfo

LIBS := $(NCURSES_LIBS)

.PHONY: dirs clean

SRC_FILES := $(wildcard $(addsuffix /*.$(SRC_EXT), $(SRC_PATH)))
SRC_OBJECTS := $(addprefix $(BUILD_PATH)/, $(notdir $(SRC_FILES:.$(SRC_EXT)=.o)))

all: dirs $(BIN_PATH)/$(BIN_NAME)

$(BIN_PATH)/$(BIN_NAME): $(SRC_OBJECTS)
	$(CC) $(COMPILE_FLAGS) $^ -o $@ $(LIBS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	$(CC) $(COMPILE_FLAGS) $^ -c -o $@

dirs:
	@mkdir $(BUILD_PATH) -p
	@mkdir $(BIN_PATH) -p

clean:
	@rm -rf $(BIN_PATH)
	@rm -rf $(BUILD_PATH)
