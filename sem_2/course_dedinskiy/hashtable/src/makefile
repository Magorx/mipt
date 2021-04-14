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

tabler_un_opt: ./un_opt/main.cpp hashtable_un_opt.o ./un_opt/ht_tests.h ./un_opt/ht_node.h
	$(CPP) $(CFLAGS) -c ./un_opt/main.cpp -o main_un_opt.o
	$(CPP) $(CFLAGS) main_un_opt.o hashtable_un_opt.o -o tabler_un_opt

hashtable_un_opt.o: ./un_opt/hashtable.cpp ./un_opt/hashtable.h ./un_opt/ht_node.h
	$(CPP) $(CFLAGS) ./un_opt/hashtable.cpp -c -o hashtable_un_opt.o

# =================================================================================

run: tabler
	./$(CUR_PROG)

tabler: ./opt/main.cpp hashtable.o ./opt/ht_tests.h ./opt/ht_node.h
	$(CPP) $(CFLAGS) -c ./opt/main.cpp -o main.o
	$(CPP) $(CFLAGS) main.o hashtable.o findopt.o -o tabler

hashtable.o: comp_asm ./opt/hashtable.cpp ./opt/hashtable.h ./opt/ht_node.h
	$(CPP) $(CFLAGS) ./opt/hashtable.cpp -c -o hashtable.o

clear:
	rm *.o

full_clear:
	rm *.o
	rm tabler
	rm tabler_un_opt

comp_asm: find_optimize.s
	nasm -felf64 find_optimize.s -o findopt.o