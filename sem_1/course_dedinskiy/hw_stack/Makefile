CC = gcc

WARNINGS = -Wall -Werror	
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS)

all: stack

stack: main.o
	$(CC) $(CFLAGS) main.o calculator.o -lm general.o -o calculator

calculator.o: calculator.c
	$(CC) $(CFLAGS) calculator.c -o calculator.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main.o

general.o: general.c
	$(CC) $(CFALGS) general.c -o general.o

run: calculator
	make clear
	./calculator

clear:
	rm *.o