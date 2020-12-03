CUR_PROG = differ

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

all: differ

differ: main.cpp expression_tree.h expression_node.h expression_node_decender.h gc_strings_and_files.o ops.dsl lang.dsl undef_lang.h latex_parts.h
	$(CPP) $(CFLAGS) main.cpp gc_strings_and_files.o -o differ

gc_strings_and_files.o: $(GC)/strings_and_files.c $(GC)/strings_and_files.h
	$(CPP) $(CFLAGS) $(GC)/strings_and_files.c -c -o gc_strings_and_files.o

run: all
	./$(CUR_PROG)

valg: all
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(CUR_PROG)
