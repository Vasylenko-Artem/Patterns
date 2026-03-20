CPP := g++
CPPFLAGS := -std=c++20 -g -Wall -Wextra -Wpedantic -Wshadow -Wunused-variable -Wuninitialized -Wconversion -Wdeprecated-declarations -Wformat -Wswitch -Wvla -Wunreachable-code -fsanitize=address -MMD -MP

INCLUDES := -Iinclude -Isrc
LDFLAGS := -lncurses 

BUILD_DIR := build

SRC_DIR := src
TARGET := $(BUILD_DIR)/main

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

.PHONY: all build run clean rebuild

all: build run

build: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(OBJS) $(LDFLAGS) -o $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS)

rebuild: clean build

run: build
	@$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)
