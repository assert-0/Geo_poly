CXX = g++
CXXFLAGS = -Wall -std=c++17

INCLUDE_DIRS = -I./include
LIB_DIRS = -L./lib

SOURCES = src/main.cxx
LIBRARIES = -lGeographic
OUTPUT_NAME = -o bin/Geo_poly.exe

all:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(LIB_DIRS) $(SOURCES) $(LIBRARIES) $(OUTPUT_NAME)