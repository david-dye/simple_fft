# Compiler
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -I./src

# Directories
SRC_DIR := src
TEST_DIR := test
BUILD_DIR := build
BIN_DIR := bin

# Find all source files recursively
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
TEST_FILES := $(shell find $(TEST_DIR) -name '*.cpp')

# Map src/*.cpp -> build/*.o (preserve directory structure)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Map test/*.cpp -> bin/<filename>
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%,$(TEST_FILES))

# Default target
all: $(OBJ_FILES) $(TEST_BINS)

# Compile source files into build directory (preserve structure)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build each test executable and link with all object files
$(BIN_DIR)/%: $(TEST_DIR)/%.cpp $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< $(OBJ_FILES) -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean