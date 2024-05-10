# Compiler to use
CC = gcc

# Flags for compiler
CFLAGS = -std=c11 -Wall -Wconversion -lm -I. -D_POSIX_C_SOURCE=200809L -DDEBUG

# Directories
BUILD_DIR = build

# Auto-detect source files
SRC_FILES = $(wildcard *.c)

# Define object files based on source files
OBJ_FILES = $(SRC_FILES:%.c=$(BUILD_DIR)/%.o)

# Name of the executable file
EXEC = $(BUILD_DIR)/game

# Default target
all: $(EXEC)

# Ensure the build directory exists
$(BUILD_DIR):
	@mkdir -p $@

# Rule to link the main program
$(EXEC): $(OBJ_FILES) | $(BUILD_DIR)
	$(CC) $(OBJ_FILES) -o $@ $(CFLAGS)

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Target to clean the build files
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(EXEC)

# Include dependency information generated by GCC
DEPS := $(OBJ_FILES:.o=.d)
-include $(DEPS)

# Phony targets
.PHONY: all clean
