CUR_PROG = general

ifndef VERBOSE
.SILENT:
endif

CUR_PROG = desee

G_PRE = general
GC = $(G_PRE)/general_c
GCPP = $(G_PRE)/general_cpp

CC = gcc
CPP = g++

WARNINGS = -Wall -Wextra -Wno-multichar
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS) -lm

all: desee

desee: main.cpp general.h decision_tree gc_strings_and_files.oo gcpp_string.oo
	$(CPP) $(CFLAGS) main.cpp decision_tree gc_strings_and_files gcpp_string -o desee

decision_tree: decision_tree.cpp decision_tree.h
	$(CPP) $(CFLAGS) decision_tree.cpp -c -o decision_tree

gc_strings_and_files.oo: $(GC)/strings_and_files.c $(GC)/strings_and_files.h
	$(CPP) $(CFLAGS) $(GC)/strings_and_files.c -c -o gc_strings_and_files.oo

gcpp_string.oo: $(GCPP)/string.cpp $(GCPP)/string.h
	$(CPP) $(CFLAGS) $(GCPP)/string.cpp -c -o gcpp_string.oo

run: all
	./$(CUR_PROG)

clear:
	rm *.o