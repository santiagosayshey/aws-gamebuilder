CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I./src
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Find all cpp files recursively
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
# Generate .o files in build directory while maintaining directory structure
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# Directories for object files
OBJ_DIRS := $(sort $(dir $(OBJS)))

# Final executable
TARGET = $(BIN_DIR)/blackjack

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIRS)
	@mkdir -p $(BUILD_DIR)/ui
	@mkdir -p $(BUILD_DIR)/states

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Clean and rebuild
rebuild: clean all

# Run the game
run: all
	./$(TARGET)

.PHONY: all clean rebuild run directories