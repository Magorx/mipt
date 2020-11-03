CUR_PROG = list

all: list
	make clear
	./$(CUR_PROG)

ifndef VERBOSE
.SILENT:
endif

CC = gcc

WARNINGS = -Wall -Wno-multichar
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS) -lm

list: main.c list.h general.h
	$(CC) $(CFLAGS) main.c -o list

valg: list
	make clear
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s ./$(CUR_PROG)

test: list_tests
	./list_tests
	make clear

list_tests: list_tests.c list_tests.h general.h list.h
	$(CC) $(CFLAGS) list_tests.c -o list_tests

clear:
	rm *.o -f