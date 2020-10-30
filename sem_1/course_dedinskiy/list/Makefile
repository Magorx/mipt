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
CFLAGS = $(STANDARD) $(WARNINGS)

list: main.c list.h general.h
	$(CC) $(CFLAGS) main.c -o list -lm

valg: list
	make clear
	valgrind --leak-check=full --track-origins=yes -s ./$(CUR_PROG)

clear:
	rm *.o -f