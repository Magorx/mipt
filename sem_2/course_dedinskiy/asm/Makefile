CUR_PROG = pprintf

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

all: compile

compile: printf.s
	nasm -f elf64 -l printf.lst printf.s
	ld -s -o pprintf printf.o

run: compile clear
	./$(CUR_PROG)

clear:
	rm *.lst *.o
