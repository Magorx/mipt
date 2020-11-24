CUR_PROG = differ

ifndef VERBOSE
.SILENT:
endif

CC  = gcc
CPP = g++

WARNINGS = -Wall -Wno-multichar
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS) -lm

all: differ

differ: main.cpp expression_tree.h
	$(CPP) $(CFLAGS) main.cpp -o differ

run: all
	./$(CUR_PROG)
