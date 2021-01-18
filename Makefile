CXX=g++
CXXFLAGS= -g -std=c++14

PYTHON_BIN     ?= python3
PYTHON_CONFIG  := $(PYTHON_BIN)-config
PYTHON_INCLUDE ?= $(shell $(PYTHON_CONFIG) --includes)
EXTRA_FLAGS    := $(PYTHON_INCLUDE)
# NOTE: Since python3.8, the correct invocation is `python3-config --libs --embed`.
# So of course the proper way to get python libs for embedding now is to
# invoke that, check if it crashes, and fall back to just `--libs` if it does.
LDFLAGS        += $(shell if $(PYTHON_CONFIG) --ldflags --embed >/dev/null; then $(PYTHON_CONFIG) --ldflags --embed; else $(PYTHON_CONFIG) --ldflags; fi)

SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

EXE := $(BIN_DIR)/fireflow
SRC := $(filter-out $(SRC_DIR)/detection.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

LIBS= -lpfring -lpcap -llog4cpp
INCLUDES = -I./include/
.PHONY: all clean

all: $(OBJ_DIR) $(BIN_DIR) $(EXE)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

# Build fireflow
$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(LIBS) $(EXTRA_FLAGS) $(LDFLAGS) $^
	rm -r $(OBJ_DIR)

# Compile object files need for fireflow
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@	

$(OBJ_DIR)/cusum.o: $(SRC_DIR)/cusum.cpp
	$(CXX) $(CXXFLAGS) $(EXTRA_FLAGS) -c $< -o $@ 

# Cleanup
clear:
	rm -rf $(OBJ_DIR) $(EXE)
