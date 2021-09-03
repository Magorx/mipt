CUR_PROG = gruphic

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
CFLAGS = $(STANDARD) $(WARNINGS) -lm -O3
SFML_LIB = -lsfml-graphics -lsfml-window -lsfml-system

all: sfml

visu: main.cpp figure.hpp parametric_line.hpp pixel.hpp
	$(CPP) $(CFLAGS) main.cpp -o visu

run: sfml
	./$(CUR_PROG)

sfml: main.cpp sfml_extension.o
	$(CPP) $(C_FLAGS) -c main.cpp
	$(CPP) $(C_FLAGS) main.o sfml_extension.o -o $(CUR_PROG) $(SFML_LIB)

%.o : $(S)%.cpp $(S)%.h
	$(CPP) $(C_FLAGS) -c $< -o $@

clear:
	rm *.o

valg: all
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(CUR_PROG)
