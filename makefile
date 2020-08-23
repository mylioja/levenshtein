#
#	Simple makefile for building the test_main
#
TARGET   := test_main

#	Directory for the build results
BUILD_DIR := ./build

SRC      := $(wildcard *.cpp)
HEADERS  := $(wildcard *.h)
OBJECTS  := $(SRC:%.cpp=$(BUILD_DIR)/%.o)

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -DDEBUG -O0 -g

all: $(BUILD_DIR)/$(TARGET)

#	Do a full build if any header is updated
$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all clean

clean:
	-@rm -rvf $(BUILD_DIR)/*
