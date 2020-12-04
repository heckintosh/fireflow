CXX=g++
CXXFLAGS= -g -std=c++14

SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

EXE := $(BIN_DIR)/fireflow
SRC := $(filter-out $(SRC_DIR)/detection.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

LIBS= -lpfring -lpcap -llog4cpp

.PHONY: all clean

all: $(EXE)

# Build fireflow
$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(LIBS) $^

# Compile object files need for fireflow
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@	

# Cleanup
clear:
	rm $(OBJ) $(EXE)
