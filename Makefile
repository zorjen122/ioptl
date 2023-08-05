CC       := g++
CFLAGS   := -Wall -Werror -Wextra --pedantic --std=c++20 -Wno-unused -Wno-error=deprecated-copy

SRC_DIR  := ./src/cpp2x/test
BUILD    := build/obj
TARGET   := test

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/%.o, $(SRCS))

.PHONY: all clean

all: $(OBJS)
	@echo "Compilation complete."

$(BUILD)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
