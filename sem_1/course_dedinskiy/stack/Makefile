ifndef VERBOSE
.SILENT:
endif

CC = gcc

WARNINGS = -Wall -Werror	
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS)

stack: main.cpp
	$(CC) $(CFLAGS) main.cpp -o stack.out

run: stack
	make clear
	./stack.out

clear:
	rm *.o -f