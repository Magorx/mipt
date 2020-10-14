ifndef VERBOSE
.SILENT:
endif

CC = gcc

WARNINGS = -Wall -Werror	
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS)

stack: main.cpp
	$(CC) $(CFLAGS) main.cpp -o stack

run: stack
	make clear
	./stack

clear:
	rm *.o -f