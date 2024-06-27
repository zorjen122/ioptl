# CFLAGS   := -Wall -Werror -Wextra --pedantic --std=c++17 -Wno-unused -Wno-error=deprecated-copy

# Makefile for compiling C++ test files in multiple directories

# Compiler and flags
CC = g++
CFLAGS = -std=c++17

# ANSI color codes
GREEN = \033[92m
RESET = \033[0m

# Directories and files

TEST_DIR_PREFIX = ./include/cpp17/test/
TEST_DIRS := allocator_test container_test impl_test iterator_test util_test
BUILD_DIR := build

# Find all .cpp files in each TEST_DIR
TEST_SOURCES := $(foreach dir,$(TEST_DIR_PREFIX + TEST_DIRS),$(wildcard $(dir)/*.cpp))
TEST_EXECUTABLES := $(patsubst %.cpp,$(BUILD_DIR)/%,$(notdir $(TEST_SOURCES)))

# Default target
all: @echo $(TEST_EXECUTABLES)

# Compile each test directory separately
$(BUILD_DIR)/%: %.cpp
	@echo "$(GREEN)########### Compiling $< ...$(RESET)"
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $< -o $@
	@echo "$(GREEN)########### Compilation completed: $@$(RESET)"

# Clean build directory
clean:
	@echo "$(GREEN)Cleaning up...$(RESET)"
	@rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
