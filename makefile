#
#	Simple makefile for building the test_main
#
TARGET   := test_main

#	Directory for the build results
BUILD_DIR := ./build

SRC      := $(wildcard *.cpp)
OBJECTS  := $(SRC:%.cpp=$(BUILD_DIR)/%.o)

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -DDEBUG -O0 -g

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all clean

clean:
	-@rm -rvf $(BUILD_DIR)/*
