ifndef VERBOSE
.SILENT:
endif

CC = gcc

WARNINGS = -Wall -Werror	
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS)

all: cpu
	$(CC) $(CFLAGS) cpu.o 

cpu: cpu.cpp
	$(CC) $(CFLAGS) cpu.c -o cpu

run: cpu
	make clear
	./cpu

clear:
	rm *.o -f