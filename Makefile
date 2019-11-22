BIN_NAME_CLIENT := client
BIN_NAME_SERVER := server

BIN_PATH := bin

BUILD_PATH := build
BUILD_PATH_CLIENT := $(BUILD_PATH)/client
BUILD_PATH_SERVER := $(BUILD_PATH)/server

SRC_PATH := src
GRAPHICS_PATH := $(SRC_PATH)/graphics
LOGIC_PATH := $(SRC_PATH)/logic
NETWORK_PATH := $(SRC_PATH)/network

COMPILE_FLAGS := -g

SRC_EXT := c
CC := gcc

NCURSES_CFLAGS := -D_GNU_SOURCE -D_DEFAULT_SOURCE -I/usr/include/ncursesw
NCURSES_LIBS := -lncursesw -ltinfo

PTHREAD_LIB := -lpthread

LIBS_SERVER := $(PTHREAD_LIB)
LIBS_CLIENT := $(NCURSES_LIBS) $(PTHREAD_LIB)

.PHONY: dirs_server dirs_client clean server client

all: server client

SRC_FILES_CLIENT := $(GRAPHICS_PATH) $(LOGIC_PATH)
SRC_FILES_CLIENT := $(wildcard $(addsuffix /*.$(SRC_EXT), $(SRC_FILES_CLIENT)))
SRC_FILES_CLIENT += $(SRC_PATH)/main_client.c  $(NETWORK_PATH)/client.c
SRC_OBJECTS_CLIENT := $(addprefix $(BUILD_PATH_CLIENT)/, $(notdir $(SRC_FILES_CLIENT:.$(SRC_EXT)=.o)))

client: dirs_client $(BIN_PATH)/$(BIN_NAME_CLIENT)

$(BIN_PATH)/$(BIN_NAME_CLIENT): $(SRC_OBJECTS_CLIENT)
	$(CC) $(COMPILE_FLAGS) $^ -o $@ $(LIBS_CLIENT)

VPATH := $(SRC_PATH) $(GRAPHICS_PATH) $(LOGIC_PATH) $(NETWORK_PATH)
$(BUILD_PATH_CLIENT)/%.o: %.$(SRC_EXT)
	$(CC) $(COMPILE_FLAGS) $^ -c -o $@

SRC_FILES_SERVER := $(LOGIC_PATH)
SRC_FILES_SERVER := $(wildcard $(addsuffix /*.$(SRC_EXT), $(SRC_FILES_SERVER)))
SRC_FILES_SERVER += $(SRC_PATH)/main_server.c  $(NETWORK_PATH)/server.c
SRC_OBJECTS_SERVER := $(addprefix $(BUILD_PATH_SERVER)/, $(notdir $(SRC_FILES_SERVER:.$(SRC_EXT)=.o)))

server: dirs_server $(BIN_PATH)/$(BIN_NAME_SERVER)

$(BIN_PATH)/$(BIN_NAME_SERVER): $(SRC_OBJECTS_SERVER)
	$(CC) $(COMPILE_FLAGS) $^ -o $@ $(LIBS_SERVER)

# VPATH := $(SRC_PATH) $(LOGIC_PATH) $(NETWORK_PATH)
$(BUILD_PATH_SERVER)/%.o: %.$(SRC_EXT)
	$(CC) $(COMPILE_FLAGS) $^ -c -o $@

dirs_client:
	@mkdir $(BUILD_PATH_CLIENT) -p
	@mkdir $(BIN_PATH) -p

dirs_server:
	@mkdir $(BUILD_PATH_SERVER) -p
	@mkdir $(BIN_PATH) -p

clean:
	@rm -rf $(BIN_PATH)
	@rm -rf $(BUILD_PATH)