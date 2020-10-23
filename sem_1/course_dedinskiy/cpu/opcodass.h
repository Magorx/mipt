#define CHECK_STACK_NOT_EMPTY() do {VERIFY(Stack_size_double(cake->rsp) >  0);} while(0)
#define CHECK_STACK_TWO_ELEMS() do {VERIFY(Stack_size_double(cake->rsp) >= 2);} while(0)
#define POP() CPU_stack_pop(cake)
#define PUSH(val) CPU_stack_push(cake, val)
#define DOUBLE_VAL double val = 0
#define READ_DOUBLE() CPU_read_value(cake, &val)
#define BYTE_VAL   byte reg_idx = 0;
#define READ_BYTE() ByteIP_get_byte(cake->bip, &reg_idx)
#define OUT() printf("%lg\n", CPU_stack_pop(cake))
#define VAL_IN() scanf("%lg", &val)

OPDEF(push, 1, 1)
OPDEF(pop, 2, 1)
OPDEF(dup, 3, 0)
OPDEF(op, 10, 1)
OPDEF(add, 11, 0)
OPDEF(sub, 12, 0)
OPDEF(mul, 13, 0)
OPDEF(div, 14, 0)
OPDEF(sin, 20, 0)
OPDEF(cos, 21, 0)

OPDEF(sqrt, 22, 0, {
	VERIFY(Stack_size_double(cake->rsp) > 0);
	
	double val = CPU_stack_pop(cake);
	CPU_stack_push(cake, sqrt(val));

	return 0;
})

OPDEF(in, 50, 0)
OPDEF(out, 51, 0)
OPDEF(jmp, 101, VALUE_LABEL)
OPDEF(ja, 102, VALUE_LABEL)
OPDEF(jae, 103, VALUE_LABEL)
OPDEF(jb, 104, VALUE_LABEL)
OPDEF(jbe, 105, VALUE_LABEL)
OPDEF(je, 106, VALUE_LABEL)
OPDEF(jne, 107, VALUE_LABEL)
OPDEF(call, 108, VALUE_LABEL)
OPDEF(ret, 109, 0)
OPDEF(halt, 255, 0)
