CUR_PROG = general

ifndef VERBOSE
.SILENT:
endif

CUR_PROG = desee

CPP = g++

WARNINGS = -Wall -Wno-multichar
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS) -lm

all: desee

desee: main.cpp general.h decision_tree
	$(CPP) $(CFLAGS) main.cpp decision_tree -o desee

decision_tree: decision_tree.cpp decision_tree.h
	$(CPP) $(CFLAGS) decision_tree.cpp -o desee

run: all
	./$(CUR_PROG)

clear:
	rm *.o