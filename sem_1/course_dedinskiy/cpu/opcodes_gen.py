#!/bin/python3

fout = open('opcodes.h', 'w')

opnames = {
   #'name' : [opcode, args_cnt],	
	'push' : [1,   1],
	'pop'  : [2,   1],
	'add'  : [10,  0],
	'sub'  : [11,  0],
	'mul'  : [12,  0],
	'div'  : [13,  0],
	'sin'  : [20,  0],
	'cos'  : [21,  0],
	'sqrt' : [22,  0],
	'in'   : [50,  0],
	'out'  : [51,  0],
	'halt' : [255, 0]
}

opcodes = [[0, ''] for _ in range(256)]
for i in range(len(opcodes)):
	for opname in opnames:
		args = opnames[opname]
		if args[0] == i:
			cnt = args[1]
			if opcodes[i][0]:
				print('Similar opcodes!', i)
				exit()

			opcodes[i][0] = i
			opcodes[i][1] = opname

upper_h = open('opcodes_h_upper.h').read()
lower_h = open('opcodes_h_lower.h').read()

print(upper_h, file=fout)

# Generation of opcodes, opnames and opargs_count =============================

# Opcodes

print("enum OPCODES {", file=fout)

for code in opcodes:
	if code[0]:
		print('    OPCODE_{} = {},'.format(code[1].upper(),code[0]), file=fout)

print("};\n", file=fout)

# Opnames

print("const char *OPNAMES[] = {", file=fout)

for code in opcodes:
	if code[0]:
		print('    [{}] = "{}",'.format(code[0], code[1]), file=fout)

print("};\n", file=fout)

# Opargs

print("const byte OPARGS[] = {", file=fout)

for code in opcodes:
	if code[0]:
		print('    [{}] = {},'.format(code[0], opnames[code[1]][1]), file=fout)

print("};\n", file=fout)

#==============================================================================

print(lower_h, file=fout)

fout.close()
