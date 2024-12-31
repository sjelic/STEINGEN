# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g -pg -O0
# CFLAGS = -O3 -march=native -DNDEBUG -Wall -Wextra -Werror -Iinclude -s -flto -fno-math-errno
LDFLAGS = -lm -flto

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Source and Object Files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Binaries
MAIN_BIN = $(BUILD_DIR)/main

# Targets
all: $(MAIN_BIN)

# Main Program
$(MAIN_BIN): $(OBJ_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# Create Build Directories
$(BUILD_DIR):
	mkdir -p $@
	
# Generate docs
docs:
	doxygen Doxyfile

# Clean
clean:
	rm -rf $(BUILD_DIR)


.PHONY: all tests clean run-tests
