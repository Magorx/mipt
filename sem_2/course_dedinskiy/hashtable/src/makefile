CUR_PROG = tabler
OPTIMIZE = ./un_opt

ifndef VERBOSE
.SILENT:
endif

GENERAL_PREFIX = general
GC = $(GENERAL_PREFIX)/c
GCPP = $(GENERAL_PREFIX)/cpp

CC = gcc
CPP = g++

WARNINGS = -Wall -Wextra -Wno-multichar
STANDARD = -std=c++17
CFLAGS = $(STANDARD) $(WARNINGS) -lm -O1 -march=native -masm=intel

all: tabler

test:
	make tabler_un_opt
	make tabler

	echo [RUN] Not optimized
	make run_un_opt
	echo -----

	echo [RUN] Optimized
	make run

run_un_opt: tabler_un_opt
	./tabler_un_opt

tabler_un_opt: main.cpp hashtable_un_opt.o ./un_opt/ht_tests.h ./un_opt/ht_node.h
	$(CPP) $(CFLAGS) -c main.cpp -o main.o
	$(CPP) $(CFLAGS) main.o hashtable_un_opt.o -o tabler_un_opt

hashtable_un_opt.o.o: ./un_opt/hashtable.cpp ./un_opt/hashtable.h ./un_opt/ht_node.h
	$(CPP) $(CFLAGS) ./un_opt/hashtable.cpp -c -o hashtable_un_opt.o

# =================================================================================

run: tabler
	./$(CUR_PROG)

tabler: main.cpp hashtable.o ./opt/ht_tests.h ./opt/ht_node.h
	$(CPP) $(CFLAGS) -c main.cpp -o main.o
	$(CPP) $(CFLAGS) main.o hashtable.o -o tabler

hashtable.o: ./opt/hashtable.cpp ./opt/hashtable.h ./opt/ht_node.h
	$(CPP) $(CFLAGS) ./opt/hashtable.cpp -c -o hashtable.o

clear:
	rm *.o

full_clear:
	rm tabler
	rm *.o