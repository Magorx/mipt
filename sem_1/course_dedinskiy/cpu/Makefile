CUR_PROG = assembler

ifndef VERBOSE
.SILENT:
endif

CC = gcc

WARNINGS = -Wall -Werror -Wno-multichar
STANDARD =  
CFLAGS = $(STANDARD) $(WARNINGS)

cpu: cpu.c
	$(CC) $(CFLAGS) cpu.c -o cpu

assembler: assembler.c general.h opcodes.h
	$(CC) $(CFLAGS) assembler.c -o assembler

everytime.mkdmp:
	./opcodes_gen.py

run: everytime.mkdmp $(CUR_PROG)
	make clear
	./$(CUR_PROG)

valg: cur_run
	make clear
	valgrind --leak-check=full --track-origins=yes -s ./$(CUR_PROG)

clear:
	rm *.o -f

show:
	hexdump -C out.kctf