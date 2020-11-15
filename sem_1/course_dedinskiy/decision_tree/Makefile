CUR_PROG = general

ifndef VERBOSE
.SILENT:
endif

CUR_PROG = desee

GENERAL_PREFIX = general
GC = $(GENERAL_PREFIX)/general_c
GCPP = $(GENERAL_PREFIX)/general_cpp

CC = gcc
CPP = g++

WARNINGS = -Wall -Wextra -Wno-multichar
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS) -lm

all: desee

desee: main.cpp general.h decision_tree.oo gc_strings_and_files.oo
	$(CPP) $(CFLAGS) main.cpp decision_tree.oo gc_strings_and_files.oo -o desee

decision_tree.oo: decision_tree.cpp decision_tree.h $(GCPP)/vector.h
	$(CPP) $(CFLAGS) decision_tree.cpp -c -o decision_tree.oo

gc_strings_and_files.oo: $(GC)/strings_and_files.c $(GC)/strings_and_files.h
	$(CPP) $(CFLAGS) $(GC)/strings_and_files.c -c -o gc_strings_and_files.oo

gcpp_string.oo: $(GCPP)/string.cpp $(GCPP)/string.h
	$(CPP) $(CFLAGS) $(GCPP)/string.cpp -c -o gcpp_string.oo

run: all
	./$(CUR_PROG)

clear:
	rm *.o