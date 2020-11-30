CUR_PROG = prog_name

ifndef VERBOSE
.SILENT:
endif

GENERAL_PREFIX = general
GC = $(GENERAL_PREFIX)/c
GCPP = $(GENERAL_PREFIX)/cpp

CC = gcc
CPP = g++

WARNINGS = -Wall -Wextra -Wno-multichar
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS) -lm

all: prog_name

prog_name: main.cpp 
	$(CPP) $(CFLAGS) main.cpp -o prog_name

run: all
	./$(CUR_PROG)

clear:
	rm *.o

valg: all
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(CUR_PROG)
