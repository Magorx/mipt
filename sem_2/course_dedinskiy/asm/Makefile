CUR_PROG = out

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

all: run

run:
	nasm -f elf64 -o pprintf.o printf.s 
	g++ -c main.cpp -o main.o
	g++ main.o pprintf.o -o out -no-pie
	./$(CUR_PROG)

clear:
	rm *.o
