CUR_PROG = tabler

# ifndef VERBOSE
# .SILENT:
# endif

GENERAL_PREFIX = general
GC = $(GENERAL_PREFIX)/c
GCPP = $(GENERAL_PREFIX)/cpp

CC = gcc
CPP = g++

WARNINGS = -Wall -Wextra -Wno-multichar
STANDARD = -std=c++17
CFLAGS = $(STANDARD) $(WARNINGS) -lm -O1 -march=native -masm=intel

all: tabler

run: tabler
	./$(CUR_PROG)

tabler: main.cpp hashtable.o ht_tests.h ht_node.h
	$(CPP) $(CFLAGS) -c main.cpp -o main.o
	$(CPP) $(CFLAGS) main.o hashtable.o -o tabler

hashtable.o: hashtable.cpp hashtable.h ht_node.h
	$(CPP) $(CFLAGS) hashtable.cpp -c -o hashtable.o

clear:
	rm *.o

full_clear:
	rm tabler
	rm *.o