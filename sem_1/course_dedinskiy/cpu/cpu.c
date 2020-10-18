#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "general.h"
#include "cpu.h"
#include "metainf.h"
#include "opcodes.h"

#define STACK_VALUE_TYPE double
#define STACK_VALUE_PRINTF_SPEC "%lg"
#include "stack.h"
#undef STACK_VALUE_TYPE
#undef STACK_VALUE_PRINTF_SPEC

typedef struct Thread_t {
	Stack_double *stack;
	Register registers[REGISTERS_COUNT];
	ByteIP *bip;
} Thread;

Thread *new_Thread(ByteIP *bip) {
	Thread *thread = calloc(1, sizeof(Thread));
	VERIFY_t(thread != NULL, Thread*);

	thread->stack = Stack_new_double();
	thread->bip = bip;

	VERIFY_t(thread->stack != NULL, Thread*);
	VERIFY_t(thread->bip   != NULL, Thread*);

	return thread;
}

int delete_Thread(Thread *cake) {
	VERIFY(cake != NULL);

	Stack_delete_double(cake->stack);
	delete_ByteIP(cake->bip);
	free(cake);

	return 0;
}

double Thread_stack_top(Thread *cake) {
	VERIFY(cake != NULL);
	VERIFY(cake->stack->size > 0);
	
	double val = Stack_top_double(cake->stack);
	Stack_pop_double(cake->stack);

	return val;
}

int Thread_stack_push(Thread *cake, const double value) {
	VERIFY(cake != NULL);

	Stack_push_double(cake->stack, value);
	return 0;
}
 
typedef struct CPU_t {
	Register registers[REGISTERS_COUNT];
	ByteIP *signature_reader;
	
	Thread **threads;
	size_t threads_capacity;
	size_t threads_size;
	size_t next_thread;
} CPU;

CPU *new_CPU() {
	CPU *cpu = calloc(1, sizeof(CPU));

	for (byte i = 0; i < REGISTERS_COUNT; ++i) {
		cpu->registers[i] = 0.0;
	}

	VERIFY_t((cpu->signature_reader = new_ByteIP(sizeof(Signature))) != NULL, CPU*);

	cpu->threads_capacity = 5;
	cpu->threads_size = 0;
	cpu->next_thread = 0;
	VERIFY_t((cpu->threads = calloc(sizeof(Thread), cpu->threads_capacity)) != NULL, CPU*);

	return cpu;
}

int delete_CPU(CPU *cake) {
	VERIFY(cake != NULL);
	
	free(cake->threads);
	delete_ByteIP(cake->signature_reader);
	free(cake);

	return 0;
}

int CPU_read_value(CPU *cake, const Thread *thread, double *value) {
	const byte symb = thread->bip->buffer[thread->bip->cur_idx];

	thread->bip->cur_idx += 1;
	DEBUG(1) printf("Reading from byte %.2x\n", symb);
	if (symb == VALUE_CONSTANT) {
		DEBUG(1) printf("const\n");
		ByteIP_get_double(thread->bip, value);
	} else if (symb == VALUE_REGISTER) {
		DEBUG(1) printf("register\n");
		byte reg_idx = 0;
		ByteIP_get_byte(thread->bip, &reg_idx);
		DEBUG(1) printf("[%.2x] -> %lg\n", reg_idx, cake->registers[reg_idx]);
		*value = cake->registers[reg_idx];
	} else {
		if (symb == OPNAME_PLUS) {
			double val_1 = 0;
			double val_2 = 0;
			CPU_read_value(cake, thread, &val_1);
			CPU_read_value(cake, thread, &val_2);
			*value = val_1 + val_2;
		} else if (symb == OPNAME_MINUS) {
			double val_1 = 0;
			double val_2 = 0;
			CPU_read_value(cake, thread, &val_1);
			CPU_read_value(cake, thread, &val_2);
			*value = val_1 - val_2;
		} else if (symb == OPNAME_MULTIPLY) {
			double val_1 = 0;
			double val_2 = 0;
			CPU_read_value(cake, thread, &val_1);
			CPU_read_value(cake, thread, &val_2);
			*value = val_1 * val_2;
		} else if (symb == OPNAME_DIVIDE) {
			double val_1 = 0;
			double val_2 = 0;
			CPU_read_value(cake, thread, &val_1);
			CPU_read_value(cake, thread, &val_2);
			*value = val_1 / val_2;
		}
	}

	return 0;
}

int CPU_execute_push(CPU *cake, Thread *thread) {
	double val = 0;
	CPU_read_value(cake, thread, &val);
	DEBUG(1) printf("pushing val = %lg\n", val);
	Thread_stack_push(thread, val);

	return 0;
}

int CPU_execute_out(CPU *cake, const Thread *thread) {
	printf("%lg\n", Stack_top_double(thread->stack));

	return 0;
}

int CPU_execute_pop(CPU *cake, Thread *thread) {
	VERIFY(thread->stack->size > 0);

	byte reg_idx = 0;
	ByteIP_get_byte(thread->bip, &reg_idx);
	ByteIP_get_byte(thread->bip, &reg_idx);

	cake->registers[reg_idx] = Thread_stack_top(thread);
	DEBUG(1) printf("pushed %lg in %.2x\n", cake->registers[reg_idx], reg_idx);

	return 0;
}

int CPU_execute_add(CPU *cake, Thread *thread) {
	VERIFY(thread->stack->size >= 2);

	double val_1 = Thread_stack_top(thread);
	double val_2 = Thread_stack_top(thread);
	Thread_stack_push(thread, val_1 + val_2);

	return 0;
}

int CPU_execute_sub(CPU *cake, Thread *thread) {
	VERIFY(thread->stack->size >= 2);

	double val_1 = Thread_stack_top(thread);
	double val_2 = Thread_stack_top(thread);
	Thread_stack_push(thread, val_1 - val_2);

	return 0;
}

int CPU_execute_mul(CPU *cake, Thread *thread) {
	VERIFY(thread->stack->size >= 2);

	double val_1 = Thread_stack_top(thread);
	double val_2 = Thread_stack_top(thread);
	Thread_stack_push(thread, val_1 * val_2);

	return 0;
}

int CPU_execute_div(CPU *cake, Thread *thread) {
	VERIFY(thread->stack->size >= 2);

	double val_1 = Thread_stack_top(thread);
	double val_2 = Thread_stack_top(thread);
	Thread_stack_push(thread, val_1 / val_2);

	return 0;
}

int CPU_execute_sin(CPU *cake, Thread *thread) {
	VERIFY(thread->stack->size > 0);
	
	double val = Thread_stack_top(thread);
	Thread_stack_push(thread, sin(val));


	return 0;
}

int CPU_execute_cos(CPU *cake, Thread *thread) {
	VERIFY(thread->stack->size > 0);
	
	double val = Thread_stack_top(thread);
	Thread_stack_push(thread, cos(val));

	return 0;
}

int CPU_execute_sqrt(CPU *cake, Thread *thread) {
	VERIFY(thread->stack->size > 0);
	
	double val = Thread_stack_top(thread);
	Thread_stack_push(thread, sqrt(val));

	return 0;
}

int CPU_execute_in(CPU *cake, Thread *thread) {
	double val = 0;
	scanf("%lg", &val);
	Thread_stack_push(thread, val);

	return 0;
}

int CPU_execute_command(CPU *cake, Thread *thread) {
	if (thread->bip->cur_idx == thread->bip->size) {
		return -1;
	}

	byte op = 0;
	ByteIP_get_byte(thread->bip, &op);

	switch (op) {
		case OPCODE_PUSH:
			CPU_execute_push(cake, thread);
			break;
		case OPCODE_POP:
			CPU_execute_pop(cake, thread);
			break;
		case OPCODE_ADD:
			CPU_execute_add(cake, thread);
			break;
		case OPCODE_SUB:
			CPU_execute_sub(cake, thread);
			break;
		case OPCODE_MUL:
			CPU_execute_mul(cake, thread);
			break;
		case OPCODE_DIV:
			CPU_execute_div(cake, thread);
			break;
		case OPCODE_SIN:
			CPU_execute_sin(cake, thread);
			break;
		case OPCODE_COS:
			CPU_execute_cos(cake, thread);
			break;
		case OPCODE_SQRT:
			CPU_execute_sqrt(cake, thread);
			break;
		case OPCODE_IN:
			CPU_execute_in(cake, thread);
			break;
		case OPCODE_OUT:
			CPU_execute_out(cake, thread);
			break;
		case OPCODE_HALT:
			return -1;
			break;
		default:
			break;
	}

	return 0;
}

int CPU_add_thread(CPU *cake, const char *file_name) {
	ByteIP_read_file(cake->signature_reader, file_name, 0);
	Signature sig = {};
	ByteIP_get(cake->signature_reader, &sig, sizeof(Signature));
	printf("[SAT]<cpu>: (thread)[(version)[%d] (file_size)[%ld] (magic)[%d]]\n", sig.version, sig.file_size, sig.magic);

	ByteIP *bip = new_ByteIP((size_t) sig.file_size);
	ByteIP_read_file(bip, file_name, (size_t) sig.file_size);
	ByteIP_get(bip, &sig, sizeof(Signature));

	Thread *thread = new_Thread(bip);
	cake->threads[cake->threads_size] = thread;
	++cake->threads_size;

	return 0;
}

int CPU_tick(CPU *cake) {
	VERIFY(cake != NULL);

	size_t thr_idx = cake->next_thread;
	size_t thrs_tried = 0;
	while (thrs_tried <= cake->threads_capacity && cake->threads[thr_idx] == NULL) {
		thr_idx = (thr_idx + 1) % cake->threads_capacity;
		++thrs_tried;
	}
	Thread *thr = cake->threads[thr_idx];
	if (thr == NULL) {
		return -1;
	}

	if (CPU_execute_command(cake, thr) != OK) {
		printf("[END]<cpu>: (thread)\n");
		delete_Thread(thr);
		cake->threads[thr_idx] = NULL;
	}

	return 0;
}


int main() {
	CPU *cpu = new_CPU();
	
	CPU_add_thread(cpu, "out.kctf");
	while(CPU_tick(cpu) == OK);

	delete_CPU(cpu);

	return 0;
}
