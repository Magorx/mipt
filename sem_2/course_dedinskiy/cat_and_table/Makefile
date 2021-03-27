CUR_PROG = catable

ifndef VERBOSE
.SILENT:
endif

GENERAL_PREFIX = general
GC = $(GENERAL_PREFIX)/c
GCPP = $(GENERAL_PREFIX)/cpp

CC = gcc
CPP = g++

WARNINGS = -Wall -Wextra -Wno-multichar
STANDARD = -std=c++17
CFLAGS = $(STANDARD) $(WARNINGS) -lm -O1 -march=native

all: sfml

catable: main.cpp figure.hpp parametric_line.hpp pixel.hpp
	$(CPP) $(CFLAGS) main.cpp -o catable

run: sfml
	./$(CUR_PROG)

sfml: main.cpp color.o color_map.o
	$(CPP) $(CFLAGS) -c main.cpp
	$(CPP) $(CFLAGS) main.o color.o color_map.o -o catable -lsfml-graphics -lsfml-window -lsfml-system

color.o: color_p4.cpp color_p4.h
	$(CPP) $(CFLAGS) color_p4.cpp -c -o color.o

color_map.o: color_map_p4.cpp color_map_p4.h
	$(CPP) $(CFLAGS) color_map_p4.cpp -c -o color_map.o

clear:
	rm *.o
