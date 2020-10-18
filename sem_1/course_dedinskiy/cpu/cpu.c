#include "cpu.h"

// The cpu itself =============================================================

int main(const int argc, const char **argv) {
	const char *input_file = "out.tf";
	if (argc > 1) {
		input_file = argv[1];
	}

	CPU *cpu = new_CPU();

	CPU_init_thread(cpu, input_file);
	CPU_init_thread(cpu, input_file);
	while(CPU_tick(cpu) == OK);

	delete_CPU(cpu);

	return 0;
}

//=============================================================================

Thread *new_Thread(const byte id) {
	Thread *thread = calloc(1, sizeof(Thread));
	VERIFY_t(thread != NULL, Thread*);

	thread->rsp = NULL;
	thread->bip = NULL;
	thread->id = id;
	for (byte i = 0; i < REGISTERS_COUNT; ++i) {
		thread->registers[i] = 0.0;
	}

	return thread;
}

int delete_Thread(Thread *cake) {
	VERIFY(cake != NULL);

	free(cake);

	return 0;
}

CPU *new_CPU() {
	CPU *cpu = calloc(1, sizeof(CPU));

	for (byte i = 0; i < REGISTERS_COUNT; ++i) {
		cpu->registers[i] = 0.0;
	}

	VERIFY_t((cpu->signature_reader = new_ByteIP(sizeof(Signature))) != NULL, CPU*);

	cpu->threads_capacity = 5;
	cpu->threads_size = 0;
	cpu->next_thread = 0;
	cpu->thread_id = 0;
	VERIFY_t((cpu->threads = calloc(sizeof(Thread), cpu->threads_capacity)) != NULL, CPU*);

	cpu->rsp = NULL;

	return cpu;
}

int delete_CPU(CPU *cake) {
	VERIFY(cake != NULL);
	
	free(cake->threads);
	delete_ByteIP(cake->signature_reader);
	free(cake);

	return 0;
}

size_t CPU_stack_size(CPU *cake) {
	VERIFY_t(cake != NULL, size_t);
	return Stack_size_double(cake->rsp);
}

double CPU_stack_top(CPU *cake) {
	VERIFY(cake != NULL);
	VERIFY(CPU_stack_size(cake) > 0);
	
	double val = Stack_top_double(cake->rsp);
	Stack_pop_double(cake->rsp);

	return val;
}

int CPU_stack_push(CPU *cake, const double value) {
	VERIFY(cake != NULL);

	Stack_push_double(cake->rsp, value);
	return 0;
}

int CPU_read_value(CPU *cake, double *value) {
	const byte symb = cake->bip->buffer[cake->bip->cur_idx];

	cake->bip->cur_idx += 1;
	DEBUG(5) printf("Reading from byte %.2x\n", symb);
	if (symb == VALUE_CONSTANT) {
		DEBUG(5) printf("const\n");
		ByteIP_get_double(cake->bip, value);
	} else if (symb == VALUE_REGISTER) {
		DEBUG(5) printf("register\n");
		byte reg_idx = 0;
		ByteIP_get_byte(cake->bip, &reg_idx);
		DEBUG(5) printf("[%.2x] -> %lg\n", reg_idx, cake->registers[reg_idx]);
		*value = cake->registers[reg_idx];
	} else {
		if (symb == OPNAME_PLUS) {
			double val_1 = 0;
			double val_2 = 0;
			CPU_read_value(cake, &val_1);
			CPU_read_value(cake, &val_2);
			*value = val_1 + val_2;
		} else if (symb == OPNAME_MINUS) {
			double val_1 = 0;
			double val_2 = 0;
			CPU_read_value(cake, &val_1);
			CPU_read_value(cake, &val_2);
			*value = val_1 - val_2;
		} else if (symb == OPNAME_MULTIPLY) {
			double val_1 = 0;
			double val_2 = 0;
			CPU_read_value(cake, &val_1);
			CPU_read_value(cake, &val_2);
			*value = val_1 * val_2;
		} else if (symb == OPNAME_DIVIDE) {
			double val_1 = 0;
			double val_2 = 0;
			CPU_read_value(cake, &val_1);
			CPU_read_value(cake, &val_2);
			*value = val_1 / val_2;
		}
	}

	return 0;
}

int CPU_execute_push(CPU *cake) {
	double val = 0;
	CPU_read_value(cake, &val);
	DEBUG(5) printf("pushing val = %lg\n", val);
	CPU_stack_push(cake, val);

	return 0;
}

int CPU_execute_out(CPU *cake) {
	printf("%lg\n", Stack_top_double(cake->rsp));

	return 0;
}

int CPU_execute_pop(CPU *cake) {
	VERIFY(CPU_stack_size(cake) > 0);

	byte reg_idx = 0;
	ByteIP_get_byte(cake->bip, &reg_idx);
	VERIFY(reg_idx == VALUE_REGISTER);
	ByteIP_get_byte(cake->bip, &reg_idx);

	cake->registers[reg_idx] = CPU_stack_top(cake);
	DEBUG(5) printf("pushed %lg in %.2x\n", cake->registers[reg_idx], reg_idx);

	return 0;
}

int CPU_execute_add(CPU *cake) {
	VERIFY(CPU_stack_size(cake) >= 2);

	double val_1 = CPU_stack_top(cake);
	double val_2 = CPU_stack_top(cake);
	CPU_stack_push(cake, val_1 + val_2);

	return 0;
}

int CPU_execute_sub(CPU *cake) {
	VERIFY(CPU_stack_size(cake) >= 2);

	double val_1 = CPU_stack_top(cake);
	double val_2 = CPU_stack_top(cake);
	CPU_stack_push(cake, val_1 - val_2);

	return 0;
}

int CPU_execute_mul(CPU *cake) {
	VERIFY(CPU_stack_size(cake) >= 2);

	double val_1 = CPU_stack_top(cake);
	double val_2 = CPU_stack_top(cake);
	CPU_stack_push(cake, val_1 * val_2);

	return 0;
}

int CPU_execute_div(CPU *cake) {
	VERIFY(CPU_stack_size(cake) >= 2);

	double val_1 = CPU_stack_top(cake);
	double val_2 = CPU_stack_top(cake);
	CPU_stack_push(cake, val_1 / val_2);

	return 0;
}

int CPU_execute_sin(CPU *cake) {
	VERIFY(CPU_stack_size(cake) > 0);
	
	double val = CPU_stack_top(cake);
	CPU_stack_push(cake, sin(val));


	return 0;
}

int CPU_execute_cos(CPU *cake) {
	VERIFY(CPU_stack_size(cake) > 0);
	
	double val = CPU_stack_top(cake);
	CPU_stack_push(cake, cos(val));

	return 0;
}

int CPU_execute_sqrt(CPU *cake) {
	VERIFY(Stack_size_double(cake->rsp) > 0);
	
	double val = CPU_stack_top(cake);
	CPU_stack_push(cake, sqrt(val));

	return 0;
}

int CPU_execute_in(CPU *cake) {
	double val = 0;
	printf("[   ]<   >: (thread)[%d] input\n", cake->thread_id);
	scanf("%lg", &val);
	DEBUG(5) printf("Read %lg from the keyboard\n", val);
	CPU_stack_push(cake, val);

	return 0;
}

int CPU_execute_command(CPU *cake) {
	if (cake->bip->cur_idx == cake->bip->size) {
		return -1;
	}

	byte op = 0;
	ByteIP_get_byte(cake->bip, &op);

	switch (op) {
		case OPCODE_PUSH:
			CPU_execute_push(cake);
			break;
		case OPCODE_POP:
			CPU_execute_pop(cake);
			break;
		case OPCODE_ADD:
			CPU_execute_add(cake);
			break;
		case OPCODE_SUB:
			CPU_execute_sub(cake);
			break;
		case OPCODE_MUL:
			CPU_execute_mul(cake);
			break;
		case OPCODE_DIV:
			CPU_execute_div(cake);
			break;
		case OPCODE_SIN:
			CPU_execute_sin(cake);
			break;
		case OPCODE_COS:
			CPU_execute_cos(cake);
			break;
		case OPCODE_SQRT:
			CPU_execute_sqrt(cake);
			break;
		case OPCODE_IN:
			CPU_execute_in(cake);
			break;
		case OPCODE_OUT:
			CPU_execute_out(cake);
			break;
		case OPCODE_HALT:
			return -1;
			break;
		default:
			break;
	}

	return 0;
}

int CPU_set_cpu_context(CPU *cake, const Thread *thread) {
	cake->rsp = thread->rsp;
	cake->bip = thread->bip;
	cake->thread_id  = thread->id;
	for (byte i = 0; i < REGISTERS_COUNT; ++i) {
		cake->registers[i] = thread->registers[i];
	}

	return 0;
}

int CPU_set_thread_context(CPU *cake, Thread *thread) {
	thread->rsp = cake->rsp;
	thread->bip = cake->bip;
	thread->id = cake->thread_id;
	for (byte i = 0; i < REGISTERS_COUNT; ++i) {
		thread->registers[i] = cake->registers[i];
	}

	return 0;
}

int CPU_init_thread(CPU *cake, const char *file_name) {
	ByteIP_read_file(cake->signature_reader, file_name, 0);
	Signature sig = {};
	ByteIP_get(cake->signature_reader, &sig, sizeof(Signature));
	ByteIP_reset(cake->signature_reader);
	printf("[INT]<cpu>: (thread)_new\n");

	if (sig.magic != KCTF_MAGIC) {
		printf("[ERR]<cpu>: (^^^^^^) Magic is not 'KCTF'\n");
		return 0;
	}

	Thread *thr = NULL;
	for (byte i = 0; i < cake->threads_capacity; ++i) {
		if (cake->threads[i] == NULL) {
			cake->threads[i] = new_Thread(i);
			thr = cake->threads[i];
			cake->thread_id = i;
			break;
		}
	}
	if (thr == NULL) {
		printf("[ERR]<cpu>: (^^^^^^) No more thread slots\n");
		return 0;
	}

	cake->rsp = Stack_new_double();
	if (cake->rsp == NULL) {
		printf("[ERR]<cpu>: (^^^^^^) Stack creation failed\n");
	}

	ByteIP *bip = new_ByteIP((size_t) sig.file_size);
	ByteIP_read_file(bip, file_name, (size_t) sig.file_size);
	ByteIP_get(bip, &sig, sizeof(Signature));
	cake->bip = bip;

	CPU_set_thread_context(cake, thr);
	++cake->threads_size;
	printf("[SAT]<cpu>: (thread)[%d]\n", thr->id);

	return 0;
}

int CPU_stop_thread(CPU *cake, Thread *thread) {
	VERIFY(cake != NULL);
	VERIFY(thread != NULL);
	printf("[END]<cpu>: (thread)\n");

	CPU_set_cpu_context(cake, thread);

	Stack_delete_double(cake->rsp);
	delete_ByteIP(cake->bip);
	delete_Thread(thread);

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
	cake->next_thread = thr_idx + 1;
	Thread *thr = cake->threads[thr_idx];
	if (thr == NULL) {
		return -1;
	}

	CPU_set_cpu_context(cake, thr);
	if (CPU_execute_command(cake) == OK) {
		CPU_set_thread_context(cake, thr);	
	} else {
		CPU_stop_thread(cake, thr);
		cake->threads[thr_idx] = NULL;
	}

	return 0;
}
