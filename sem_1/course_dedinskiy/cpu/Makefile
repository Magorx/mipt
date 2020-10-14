CUR_PROG = assembler

ifndef VERBOSE
.SILENT:
endif

CC = gcc

WARNINGS = -Wall	
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS)

all: cpu
	$(CC) $(CFLAGS) cpu.o 

cpu: cpu.c
	$(CC) $(CFLAGS) cpu.c -o cpu

assembler: assembler.c general.h opcodes.h
	$(CC) $(CFLAGS) assembler.c -o assembler

cur_run: $(CUR_PROG)

run: cur_run
	make clear
	./$(CUR_PROG)

valg: cur_run
	make clear
	valgrind --leak-check=full --track-origins=yes -s ./$(CUR_PROG)

clear:
	rm *.o -f