CUR_PROG = brod

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
CFLAGS = $(STANDARD) $(WARNINGS) -lm -O3 -msse4.1 

all: sfml

brod: main.cpp figure.hpp parametric_line.hpp pixel.hpp
	$(CPP) $(CFLAGS) main.cpp -o brod

run: sfml
	./$(CUR_PROG)

sfml: main.cpp vec3d.o color.o color_map.o mandelbroter.o
	$(CPP) $(CFLAGS) -c main.cpp
	$(CPP) $(CFLAGS) main.o vec3d.o color.o color_map.o mandelbroter.o -o brod -lsfml-graphics -lsfml-window -lsfml-system

vec3d.o: vec3d.cpp vec3d.h
	$(CPP) $(CFLAGS) vec3d.cpp -c

color.o: color.cpp color.h
	$(CPP) $(CFLAGS) color.cpp -c

color_map.o: color_map.cpp color_map.h
	$(CPP) $(CFLAGS) color_map.cpp -c

mandelbroter.o: mandelbroter.cpp mandelbroter.h
	$(CPP) $(CFLAGS) mandelbroter.cpp -c

clear:
	rm *.o
