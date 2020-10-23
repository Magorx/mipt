#define VERIFY_STACK_NOT_EMPTY() do {VERIFY(Stack_size_double(cake->rsp) >  0);} while(0)
#define VERIFY_STACK_HAS_TWO_ELEMS() do {VERIFY(Stack_size_double(cake->rsp) >= 2);} while(0)

#define VAL val
#define VAL_1 val_1
#define VAL_2 val_2
#define NEW_RIP new_rip
#define REG_IDX reg_idx

#define RIP cake->rip
#define RSP cake->rsp

#define POP() CPU_stack_pop(cake)
#define PUSH(val) CPU_stack_push(cake, val)

#define REGISTERS(reg_idx) cake->registers[reg_idx]

#define READ_VAL() CPU_read_value(cake, &val)
#define READ_VAL_1() CPU_read_value(cake, &val_1)
#define READ_VAL_2() CPU_read_value(cake, &val_2)
#define READ_REG_IDX() ByteIP_get_byte(cake->bip, &reg_idx)
#define READ_NEW_RIP() NEW_RIP = CPU_read_size_t(cake)

#define VERIFY_BYTE_REG(REG) VERIFY(REG == VALUE_REGISTER)

#define BINARY_OP(op, val_2, val_1) CPU_symb_operation(cake, op, val_2, val_1)

#define OUT() printf("%lg\n", CPU_stack_pop(cake))
#define IN_VAL() scanf("%lg", &val)

#define JMP(new_rip) CPU_jump(cake, new_rip);

OPDEF(push, 1, 1, {
	READ_VAL();
	PUSH(VAL);
})

OPDEF(pop, 2, 1, {
	READ_REG_IDX();
	VERIFY_BYTE_REG(REG_IDX);
	READ_REG_IDX();
	REGISTERS(REG_IDX) = POP();
})

OPDEF(dup, 3, 0, {
	VAL = POP();
	PUSH(VAL);	
	PUSH(VAL);	
})

OPDEF(bin_op, 10, 1, {
	READ_REG_IDX();
	VAL_1 = POP();
	VAL_2 = POP();
	PUSH(BINARY_OP(REG_IDX, VAL_2, VAL_1));
})

OPDEF(add, 11, 0, {
	VERIFY_STACK_HAS_TWO_ELEMS();
	PUSH(POP() + POP());
})


OPDEF(sub, 12, 0, {
	VERIFY_STACK_HAS_TWO_ELEMS();
	PUSH(-1 * POP() + POP());
})

OPDEF(mul, 13, 0, {
	VERIFY_STACK_HAS_TWO_ELEMS();
	PUSH(POP() + POP());
})	

OPDEF(div, 14, 0, {
	VERIFY_STACK_HAS_TWO_ELEMS();
	PUSH(1 / POP() * POP());	
})

OPDEF(sin, 20, 0, {
	VERIFY_STACK_NOT_EMPTY();	
	PUSH(sin(POP()));	
})

OPDEF(cos, 21, 0, {
	VERIFY_STACK_NOT_EMPTY();	
	PUSH(cos(POP()));	
})

OPDEF(sqrt, 22, 0, {	
	VERIFY_STACK_NOT_EMPTY();	
	PUSH(sqrt(POP()));	
})

OPDEF(in, 50, 0, {	
	IN_VAL();
	PUSH(VAL);
})

OPDEF(out, 51, 0, {
	OUT();
})

OPDEF(jmp, 101, VALUE_LABEL, {
	READ_NEW_RIP();
	JMP(NEW_RIP);
})

OPDEF(ja, 102, VALUE_LABEL, {
	READ_NEW_RIP();
	if (POP() < POP()) {
		JMP(NEW_RIP);
	}
})

OPDEF(jae, 103, VALUE_LABEL, {
	READ_NEW_RIP();
	if (POP() <= POP()) {
		JMP(NEW_RIP);
	}
})

OPDEF(jb, 104, VALUE_LABEL, {
	READ_NEW_RIP();
	if (POP() > POP()) {
		JMP(NEW_RIP);
	}
})

OPDEF(jbe, 105, VALUE_LABEL, {
	READ_NEW_RIP();
	if (POP() >= POP()) {
		JMP(NEW_RIP);
	}
})

OPDEF(je, 106, VALUE_LABEL, {
	READ_NEW_RIP();
	if (fabs(POP() - POP()) <= EPS) {
		JMP(NEW_RIP);
	}
})

OPDEF(jne, 107, VALUE_LABEL, {
	READ_NEW_RIP();
	if (fabs(POP() - POP()) > EPS) {
		JMP(NEW_RIP);
	}
})

OPDEF(call, 108, VALUE_LABEL, {
	READ_NEW_RIP();
	PUSH((double) *RIP);
	JMP(NEW_RIP);
})

OPDEF(ret, 109, 0, {
	NEW_RIP = (size_t) POP();
	JMP(NEW_RIP);
})

OPDEF(halt, 255, 0, {
	return -1;
})
