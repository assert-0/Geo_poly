CXX = g++
CXXFLAGS = -Wall -L./lib

SOURCES = src/main.cxx
LIBRARIES = -lGeographic
OUTPUT_NAME = -o bin/Geo_poly.exe

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LIBRARIES) $(OUTPUT_NAME)